#define _USE_MATH_DEFINES
#include "viewwidget.h"
#include <iostream>
#include <math.h>
#include <time.h>
#include "LoadShaders.h"
#include "mymath.h"

using namespace std;

enum RENDER_MODE { RENDER_POLYGON, RENDER_WIREFRAME, RENDER_PICKING };

ViewWidget::ViewWidget(QWidget * parent) : QOpenGLWidget(parent)
{
	setupOpenGL();

	iconSize = 32;

	eye[0] = 1; eye[1] = 0; eye[2] = 0;
	lok[0] = 0; lok[1] = 0; lok[2] = 0;
	vup[0] = 0, vup[1] = 1, vup[2] = 0;
	viewTrans[0] = 0; viewTrans[1] = 0; viewTrans[2] = 0;
	viewRot[0] = 0; viewRot[1] = 0; viewRot[2] = 0;
	viewPhi = radians(0);
	viewTheta = radians(60);
	viewRadius = 400.0;
	fovy = 45.0;
	nearClip = 0.1;
	farClip = 1000.0;
	fixView();

	sceneID = 0;

	// light
	setGLitAmbient(&globalLight, 0.3, 0.3, 0.3, 1.0);
	setLitPosition(&light0, 1, 1, 1, 0);
	setLitDirection(&light0, 1, 1, 1);
	setLitDiffuse(&light0, 0.4, 0.4, 0.4, 1.0);
	setLitSpecular(&light0, 0.45, 0.45, 0.45, 1.0);

	// ssao
	kernelSize = 64;
	noiseSize = 4.0;
	kernelRadius = 1.0;
	quadVAO = 0;
	ssaoKernel = (GLfloat*)calloc(kernelSize * 3, sizeof(GLfloat));
	ssaoNoise = (GLfloat*)calloc(noiseSize * noiseSize * 3, sizeof(GLfloat));


	// shadow mapping
	shadowWidth = 4096;
	shadowHeight = 4096;

	vboa = newVertexBufferObjectArray();
}

ViewWidget::~ViewWidget()
{
	free(ssaoKernel);
	free(ssaoNoise);
}

void ViewWidget::genSSAO(void)
{
	// Set samplers
	glUseProgram(program[7]);
	glUniform1i(glGetUniformLocation(program[7], "gPositionDepth"), 0);
	glUniform1i(glGetUniformLocation(program[7], "gNormal"), 1);
	glUniform1i(glGetUniformLocation(program[7], "gAlbedo"), 2);
	glUniform1i(glGetUniformLocation(program[7], "ssao"), 3);
	glUseProgram(program[5]);
	glUniform1i(glGetUniformLocation(program[5], "gPositionDepth"), 0);
	glUniform1i(glGetUniformLocation(program[5], "gNormal"), 1);
	glUniform1i(glGetUniformLocation(program[5], "texNoise"), 2);

	// Set up G-Buffer
	// 3 textures:
	// 1. Positions + depth (RGBA)
	// 2. Color (RGB) 
	// 3. Normals (RGB) 
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	// - Position + linear depth color buffer
	glGenTextures(1, &gPositionDepth);
	glBindTexture(GL_TEXTURE_2D, gPositionDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPositionDepth, 0);
	// - Normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	// - Albedo color buffer
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width(), this->height(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	// - Create and attach depth buffer (renderbuffer)
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->width(), this->height());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// - Finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "GBuffer Framebuffer not complete!\n");
	}

	// Also create framebuffer to hold SSAO processing stage 
	glGenFramebuffers(1, &ssaoFBO);
	glGenFramebuffers(1, &ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	// - SSAO color buffer
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "SSAO Framebuffer not complete!");
	}
	// - and blur stage
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glGenTextures(1, &ssaoColorBufferBlur);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "SSAO Blur Framebuffer not complete!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());

	int i;
	float factor, scale, sample[3];
	// Sample kernel

	for (i = 0; i < kernelSize; ++i)
	{
		sample[0] = randomF(2.0) - 1;
		sample[1] = randomF(2.0) - 1;
		sample[2] = randomF(1.0);
		normalize3fv(sample);
		factor = randomF(1.0);
		mult3x1fv(sample, factor, sample);
		scale = i / (float)kernelSize;

		// Scale samples s.t. they're more aligned to center of kernel
		scale = lerp(0.1f, 1.0f, scale * scale);
		mult3x1fv(sample, scale, sample);
		ssaoKernel[i * 3] = sample[0];
		ssaoKernel[i * 3 + 1] = sample[1];
		ssaoKernel[i * 3 + 2] = sample[2];
	}

	// Noise texture
	for (i = 0; i < 16; i++)
	{
		ssaoNoise[i * 3] = randomF(2.0) - 1.0;
		ssaoNoise[i * 3 + 1] = randomF(2.0) - 1.0;
		ssaoNoise[i * 3 + 2] = 0.0;
	}

	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, noiseSize, noiseSize, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void ViewWidget::genShadowMapping(void)
{
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("FBO error\n");
	}
}

void ViewWidget::genMultiSampling(void)
{
	glGenTextures(1, &multiSampleTex);
	glGenFramebuffers(1, &multiSampleFBO);
	glGenRenderbuffers(1, &multiSampleColorBuffer);
	glGenRenderbuffers(1, &multiSampleDepthBuffer);

	glEnable(GL_MULTISAMPLE);
}

void ViewWidget::setupOpenGL(void)
{
	QSurfaceFormat format = QSurfaceFormat::defaultFormat();

	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setVersion(4, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	setFocusPolicy(Qt::StrongFocus);
}

void ViewWidget::initializeGL(void)
{
	float mat[16];

	//  initialize glew
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	setupOpenGL();

	//  load shaders

	// phong shading model (object) render
	ShaderInfo modelShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\model.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\model.frag" },
		{ GL_NONE, NULL }
	};
	program[0] = LoadShaders(modelShaders);

	// shadow mapping for model render
	ShaderInfo shadowShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\shadow.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\shadow.frag" },
		{ GL_NONE, NULL }
	};
	program[1] = LoadShaders(shadowShaders);

	ShaderInfo pickingShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\picking.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\picking.frag" },
		{ GL_NONE, NULL }
	};
	program[2] = LoadShaders(pickingShaders);

	ShaderInfo simpleShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\simple.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\simple.frag" },
		{ GL_NONE, NULL }
	};
	program[3] = LoadShaders(simpleShaders);

	ShaderInfo deferredShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\ssao_geometry.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\ssao_geometry.frag" },
		{ GL_NONE, NULL }
	};
	program[4] = LoadShaders(deferredShaders);

	ShaderInfo ssaoShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\ssao.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\ssao.frag" },
		{ GL_NONE, NULL }
	};
	program[5] = LoadShaders(ssaoShaders);

	ShaderInfo ssaoBlurShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\ssao_blur.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\ssao_blur.frag" },
		{ GL_NONE, NULL }
	};
	program[6] = LoadShaders(ssaoBlurShaders);

	ShaderInfo ssaoLightPassShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\lightpass.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\lightpass.frag" },
		{ GL_NONE, NULL }
	};
	program[7] = LoadShaders(ssaoLightPassShaders);

	// use shader program
	glUseProgram(program[0]);

	// viewing varibles
	setViewingMatrix();

	// light varibles
	setLighting();

	// color varibles
	colorLoc = glGetUniformLocation(program[0], "color");

	// flag varible
	wireframe = glGetUniformLocation(program[0], "wireframe");
	glUniform1i(wireframe, 0);
	glEnable(GL_DEPTH_TEST);
	// check assignment error
	//CheckGLErrors();

	// create gound plane
	vbo_t* ground;
	ground = newPlane(200, 200, 1);
	addVertexBufferObject(ground, vboa);
	setMatAmbient(ground->material, .19225, .19225, .19225, 1.0);
	setMatDiffuse(ground->material, .50754, .50754, .50754, 1.0);
	setMatSpecular(ground->material, .508273, .508273, .508273, 1.0);
	setMatShininess(ground->material, .4 * 128.0);
	rotateX(90.0, mat);
	translate(-100, -100, 0, ground->modelMat);
	multMat4fv(mat, ground->modelMat, ground->modelMat);
	bindData(ground);

	// ssao
	genSSAO();

	// FBO depth map
	genShadowMapping();

	// multiple sample FBO
	genMultiSampling();

	// enable clip plane
	glEnable(GL_CLIP_DISTANCE0);
}

void ViewWidget::configureShadowMapping(void)
{
	float nearPlane = 1.0, farPlane = 500.0, mat[16], view[16], proj[16];
	float lit[] = { light0.position[0], light0.position[1], light0.position[2] };
	normalize3fv(lit);
	float lightPos[] = { lit[0] * 200, lit[1] * 200, lit[2] * 200 };
	float lightLok[] = { 0, 0, 0 };
	float lightVup[] = { 0, 1, 0 };

	ortho(-300.0, 300.0, -300.0, 300.0, nearPlane, farPlane, proj);
	lookAt(lightPos, lightLok, lightVup, view);
	multMat4fv(proj, view, lightSpaceMat);
	transposeMat4fv(lightSpaceMat, mat);
	glUniformMatrix4fv(6, 1, GL_FALSE, mat);
}

void ViewWidget::configureGBufferShader(void)
{

}

void ViewWidget::renderScene(void)
{
	int i;

	for (i = 0; i < vboa->numVBOs; ++i)
	{
		if (i == 0) {
			glDisable(GL_CLIP_DISTANCE0);
		}
		drawData(vboa->vbos[i], RENDER_POLYGON);
		if (i == 0) {
			glEnable(GL_CLIP_DISTANCE0);
		}
	}
}

void ViewWidget::renderBbox(void)
{
	int i;

	for (i = 0; i < vboa->numVBOs; ++i)
	{
		if (vboa->vbos[i]->bboxDetect) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_LINE_SMOOTH);
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			drawBbox(vboa->vbos[i], RENDER_WIREFRAME);
			glDisable(GL_BLEND);
		}
	}
}

void ViewWidget::setViewingMatrix(void)
{
	float mat[16], tmp[16], aspect;

	// initialize variables and assign them to shader program
	// viewing matrices
	// z-axis up

	identifyMat4fv(modelMat);						// model matrix
	transposeMat4fv(modelMat, mat);
	glUniformMatrix4fv(0, 1, false, mat);
	lookAt(eye, lok, vup, viewMat);					// view matrix
	transposeMat4fv(viewMat, mat);
	glUniformMatrix4fv(1, 1, false, mat);
	multMat4fv(viewMat, modelMat, mvMat);			// modelview matrix
	transposeMat4fv(mvMat, mat);
	glUniformMatrix4fv(3, 1, false, mat);
	normalM(mvMat, normalMat);						// normal matrix
	transposeMat4fv(normalMat, mat);
	glUniformMatrix4fv(5, 1, false, mat);
	aspect = this->width() / (float)this->height();	// projection matrix
	perspective(fovy, aspect, nearClip, farClip, projectionMat);
	transposeMat4fv(projectionMat, mat);
	glUniformMatrix4fv(2, 1, false, mat);
	multMat4fv(projectionMat, viewMat, mvpMat);
	multMat4fv(mvpMat, modelMat, mvpMat);
	transposeMat4fv(mvpMat, mat);
	glUniformMatrix4fv(4, 1, false, mat);
	glViewport(0, 0, this->width(), this->height());
}

void ViewWidget::setLighting(void)
{
	GLuint loc;

	glUniform4fv(7, 1, light0.position);
	loc = glGetUniformLocation(program[0], "lightAmbient0");
	glUniform4fv(loc, 1, globalLight.ambient);
	loc = glGetUniformLocation(program[0], "lightDiffuse0");
	glUniform4fv(loc, 1, light0.diffuse);
	loc = glGetUniformLocation(program[0], "lightSpecular0");
	glUniform4fv(loc, 1, light0.specular);
}


void ViewWidget::setMaterial(vbo_t obj)
{
	GLuint loc;

	// material varibles
	loc = glGetUniformLocation(program[0], "materialAmbient");
	glUniform4fv(loc, 1, obj.material->ambient);
	loc = glGetUniformLocation(program[0], "materialDiffuse");
	glUniform4fv(loc, 1, obj.material->diffuse);
	loc = glGetUniformLocation(program[0], "materialSpecular");
	glUniform4fv(loc, 1, obj.material->specular);
	loc = glGetUniformLocation(program[0], "shininess");
	glUniform1f(loc, obj.material->shininess);
	loc = glGetUniformLocation(program[0], "enableMaterial");
	glUniform1i(loc, obj.material->isEnable);
}

void ViewWidget::fixView(void)
{
	float temp;

	if (viewTheta < 0.01) {
		viewTheta = 0.0 + 0.001;
	}
	else if (viewTheta > M_PI) {
		viewTheta = M_PI - 0.001;
	}
	else {
		vup[0] = 0;
		vup[1] = 1;
		vup[2] = 0;
	}

	eye[0] = lok[0] + viewRadius * cos(viewPhi) * sin(viewTheta);
	eye[1] = lok[1] + viewRadius * cos(viewTheta);
	eye[2] = lok[2] + viewRadius * sin(viewPhi) * sin(viewTheta);

	lookAt(eye, lok, vup, viewMat);
}


void ViewWidget::updateViewing(void)
{
	float mat[16];

	multMat4fv(viewMat, modelMat, mvMat);
	normalM(mvMat, normalMat);
	multMat4fv(projectionMat, mvMat, mvpMat);

	transposeMat4fv(modelMat, mat);
	glUniformMatrix4fv(0, 1, false, mat);
	transposeMat4fv(viewMat, mat);
	glUniformMatrix4fv(1, 1, false, mat);
	transposeMat4fv(normalMat, mat);
	glUniformMatrix4fv(5, 1, false, mat);
	transposeMat4fv(mvMat, mat);
	glUniformMatrix4fv(3, 1, false, mat);
	transposeMat4fv(mvpMat, mat);
	glUniformMatrix4fv(4, 1, false, mat);
}

void ViewWidget::bindData(vbo_t* vbo)
{
	int i;

	if (!vbo->isGenBuffer) {
		glGenBuffers(NUM_ATTRIBS, vbo->buffers);
		glGenBuffers(NUM_ATTRIBS, vbo->bbox->buffers);
		vbo->isGenBuffer = true;
		//printf("gen: %d\n", CheckGLErrors());
	}
	for (i = 0; i < NUM_ATTRIBS; ++i)
	{
		if (vbo->enableBuffers[i]) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[i]);
			//printf("bindBuffer: %d\n", CheckGLErrors());
			glBufferData(GL_ARRAY_BUFFER, vbo->numVertices * vbo->dataSize[i] * sizeof(float), vbo->attribs[i], GL_STATIC_DRAW);
			//printf("bufferData: %d\n", CheckGLErrors());
		}
		if (vbo->bbox->enableBuffers[i]) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo->bbox->buffers[i]);
			if (i == WIREFRAME) {
				glBufferData(GL_ARRAY_BUFFER, vbo->bbox->numWFVertices * vbo->bbox->dataSize[i] * sizeof(float), vbo->bbox->attribs[i], GL_STATIC_DRAW);
			}
			else {
				glBufferData(GL_ARRAY_BUFFER, vbo->bbox->numVertices * vbo->bbox->dataSize[i] * sizeof(float), vbo->bbox->attribs[i], GL_STATIC_DRAW);
			}
		}
	}
}

void ViewWidget::drawData(vbo_t* const vbo, int mode)
{
	int i;
	float rotMat[16];

	copyMat4fv(vbo->modelMat, modelMat);

	if (mode == RENDER_PICKING) {
		if (vbo->enableBuffers[FACE]) {
			glUseProgram(program[2]);
			updateViewing();
			setLighting();
			glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[VERTEX]);
			glVertexAttribPointer(VERTEX, vbo->dataSize[VERTEX], GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(VERTEX);
			glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[FACE]);
			glVertexAttribPointer(FACE, vbo->dataSize[FACE], GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(FACE);
			glDrawArrays(GL_TRIANGLES, 0, vbo->numVertices);
		}
	}
	else if (mode == RENDER_WIREFRAME) {
		glUseProgram(program[3]);
		updateViewing();
		setLighting();
		glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[VERTEX]);
		glVertexAttribPointer(VERTEX, vbo->dataSize[VERTEX], GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(VERTEX);
		glUniform4f(0, 0.0, 0.0, 0.0, 1.0);
		glLineWidth(2.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, vbo->numVertices);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else {
		updateViewing();
		setLighting();
		setMaterial(*vbo);
		float screenSize[2];
		screenSize[0] = this->width();
		screenSize[1] = this->height();
		glUniform2fv(glGetUniformLocation(program[0], "screenSize"), 1, screenSize);
		for (i = 0; i < NUM_ATTRIBS; ++i)
		{
			if (vbo->enableBuffers[i]) {
				glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[i]);
				glVertexAttribPointer(i, vbo->dataSize[i], GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(i);
				//printf("bind: %d\n", CheckGLErrors());
			}
		}
		if (mode == RENDER_POLYGON) {
			//glEnable(GL_CULL_FACE);
			//glCullFace(GL_FRONT);
			//glPolygonMode(GL_BACK, GL_LINE);
			//glLineWidth(10.0);
			//glDrawArrays(GL_TRIANGLES, 0, vbo->numVertices);
			//glLineWidth(1.0);
			//glPolygonMode(GL_BACK, GL_FILL);
			//glDisable(GL_CULL_FACE);
			glDrawArrays(GL_TRIANGLES, 0, vbo->numVertices);

		}
	}
	//printf("draw: %d\n", CheckGLErrors());
	for (i = 0; i < NUM_ATTRIBS; ++i)
	{
		if (vbo->enableBuffers[i]) {
			glDisableVertexAttribArray(i);
		}
	}
}

void ViewWidget::drawBbox(vbo_t* const vbo, int mode)
{
	int i;
	float rotMat[16], color[4];

	copyMat4fv(vbo->modelMat, modelMat);

	// render bounding box
	if (mode == RENDER_POLYGON) {
		if (vbo->bbox->enableBuffers[VERTEX]) {
			glUseProgram(program[3]);
			updateViewing();
			setLighting();
			setMaterial(*vbo);
			for (i = 0; i < 4; ++i)
			{
				if (vbo->bbox->enableBuffers[i]) {
					glBindBuffer(GL_ARRAY_BUFFER, vbo->bbox->buffers[i]);
					glVertexAttribPointer(i, vbo->bbox->dataSize[i], GL_FLOAT, GL_FALSE, 0, NULL);
					glEnableVertexAttribArray(i);
				}
			}
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDrawArrays(GL_TRIANGLES, 0, vbo->bbox->numVertices);
			glDisable(GL_BLEND);
		}
	}
	// line loop each face
	if (mode == RENDER_WIREFRAME) {
		if (vbo->bbox->enableBuffers[WIREFRAME]) {
			glUseProgram(program[3]);
			updateViewing();
			setLighting();
			color[0] = 0.0;
			color[1] = 1.0;
			color[2] = 0.0;
			color[3] = 1.0;
			glUniform4fv(11, 1, color);
			glBindBuffer(GL_ARRAY_BUFFER, vbo->bbox->buffers[WIREFRAME]);
			glVertexAttribPointer(VERTEX, vbo->bbox->dataSize[WIREFRAME], GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(VERTEX);
			//printf("bind: %d\n", CheckGLErrors());
			for (i = 0; i < vbo->bbox->numWFVertices; i += vbo->bbox->numWFLoop)
			{
				glDrawArrays(GL_LINE_LOOP, i, vbo->bbox->numWFLoop);
			}
		}
	}
	if (mode == RENDER_PICKING) {
		if (vbo->bbox->enableBuffers[FACE]) {
			glUseProgram(program[2]);
			updateViewing();
			setLighting();
			glBindBuffer(GL_ARRAY_BUFFER, vbo->bbox->buffers[VERTEX]);
			glVertexAttribPointer(VERTEX, vbo->bbox->dataSize[VERTEX], GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(VERTEX);
			glBindBuffer(GL_ARRAY_BUFFER, vbo->bbox->buffers[FACE]);
			glVertexAttribPointer(FACE, vbo->bbox->dataSize[FACE], GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(FACE);
			glDrawArrays(GL_TRIANGLES, 0, vbo->bbox->numVertices);
		}
	}
	// close Attirbs
	for (i = 0; i < NUM_ATTRIBS; ++i)
	{
		if (vbo->bbox->enableBuffers[i]) {
			glDisableVertexAttribArray(i);
		}
	}
}

void ViewWidget::gBufferRendering(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program[4]);
	renderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
}

void ViewWidget::ssaoTextureRendering(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program[5]);
	float screenSize[2];
	screenSize[0] = this->width();
	screenSize[1] = this->height();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPositionDepth);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glUniform1i(glGetUniformLocation(program[5], "noiseSize"), noiseSize);
	glUniform1i(glGetUniformLocation(program[5], "kernelSize"), kernelSize);
	glUniform1f(glGetUniformLocation(program[5], "radius"), kernelRadius);
	glUniform2fv(glGetUniformLocation(program[5], "screenSize"), 1, screenSize);
	// Send kernel + rotation 
	char str[100] = { '\0' }, idx[10];
	for (GLuint i = 0; i < kernelSize; ++i)
	{
		strcpy(str, "samples[");
		sprintf(idx, "%d", i);
		strcat(str, idx);
		strcat(str, "]");
		glUniform3fv(glGetUniformLocation(program[5], str), 1, ssaoKernel + i * 3);
	}
	setViewingMatrix();
	renderQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
}

void ViewWidget::ssaoBlurRendering(void)
{
	// 3. Blur SSAO texture to remove noise
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program[6]);
	glUniform1i(glGetUniformLocation(program[6], "noiseSize"), noiseSize);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	renderQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
}

void ViewWidget::ssaoLightPassRendering(void)
{
	// 4. Lighting Pass: traditional deferred Blinn-Phong lighting now with added screen-space ambient occlusion
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program[7]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPositionDepth);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glActiveTexture(GL_TEXTURE3); // Add extra SSAO texture to lighting pass
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	// Also send light relevant uniforms
	// Lights
	float lightPos[3] = { 2.0, 4.0, -2.0 };
	float lightColor[3] = { 0.7, 0.7, 0.7 };
	glUniform3fv(glGetUniformLocation(program[7], "light.Position"), 1, lightPos);
	glUniform3fv(glGetUniformLocation(program[7], "light.Color"), 1, lightColor);
	// Update attenuation parameters
	const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
	const GLfloat linear = 0.09;
	const GLfloat quadratic = 0.032;
	glUniform1f(glGetUniformLocation(program[7], "light.Linear"), linear);
	glUniform1f(glGetUniformLocation(program[7], "light.Quadratic"), quadratic);
	renderQuad();
}

void ViewWidget::shadowMappingRendering(void)
{
	// 1. first render to depth map
	glUseProgram(program[1]);
	glViewport(0, 0, shadowWidth, shadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	configureShadowMapping();
	renderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());

	// 2. then render scene as normal with shadow mapping (using depth map)
	glUseProgram(program[0]);

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multiSampleTex);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 16, GL_RGBA, this->width(), this->height(), GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, multiSampleFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, multiSampleColorBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 16, GL_RGBA8, this->width(), this->height());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, multiSampleColorBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, multiSampleDepthBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 16, GL_DEPTH24_STENCIL8, this->width(), this->height());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, multiSampleDepthBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, multiSampleDepthBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multiSampleTex, 0);

}

void ViewWidget::paintGL(void)
{
	float mat[16], color[4];

	// ssao
	gBufferRendering();
	ssaoTextureRendering();
	ssaoBlurRendering();
	//ssaoLightPassRendering();

	// render shadow map

	shadowMappingRendering();

	glUseProgram(program[0]);
	glViewport(0, 0, this->width(), this->height());
	glClearColor(0.66, 0.66, 0.66, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniform1i(glGetUniformLocation(program[0], "shadowMap"), 0);
	glUniform1i(glGetUniformLocation(program[0], "ssaoMap"), 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glActiveTexture(GL_TEXTURE1); // Add extra SSAO texture to lighting pass
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	transposeMat4fv(lightSpaceMat, mat);
	glUniformMatrix4fv(6, 1, GL_FALSE, mat);
	//updateViewing();
	renderScene();
	renderBbox();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, multiSampleFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->defaultFramebufferObject());
	glDrawBuffer(GL_BACK);
	glBlitFramebuffer(0, 0, this->width(), this->height(), 0, 0, this->width(), this->height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

}

void ViewWidget::resizeSSAO(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	// - Position + linear depth color buffer
	glBindTexture(GL_TEXTURE_2D, gPositionDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// - Normal color buffer
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// - Albedo color buffer
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width(), this->height(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// - Depth buffer (renderbuffer)
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->width(), this->height());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	// - SSAO color buffer
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// - and blur stage
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
}

void ViewWidget::resizeGL(int width, int height)
{
	float mat[16];

	glViewport(0, 0, width, height);
	float aspect = width / (float)height;
	perspective(fovy, aspect, nearClip, farClip, projectionMat);
	transposeMat4fv(projectionMat, mat);
	glUniformMatrix4fv(2, 1, false, mat);

	
	update();
}

void ViewWidget::mousePressEvent(QMouseEvent *e)
{
	int i, ret;
	unsigned int objID;
	float rayDir[3];
	unsigned char pickedColor[4];

	
	switch (e->buttons())
	{
	case Qt::LeftButton:
		//cout<<"Left\n";
		break;
	case Qt::MidButton:
		//cout<<"Mid\n";
		break;
	case Qt::RightButton:
		//cout << "Right\n";
		break;
	default:
		break;
	}
	oldPosX = e->x();
	oldPosY = e->y();
	update();
}

void ViewWidget::mouseMoveEvent(QMouseEvent *e)
{
	float u[3] = { viewMat[0], viewMat[1], viewMat[2] };
	float v[3] = { viewMat[4], viewMat[5], viewMat[6] };
	float w[3] = { viewMat[8], viewMat[9], viewMat[10] };
	float trans[3], transMat[16], boxCenter[3], param[4], projPnt[3], dir[3], tmp;
	int offsetX = (e->x() - oldPosX);
	int offsetY = -(e->y() - oldPosY);
	float nWidth, nHeight, mOffX, mOffY;

	
	switch (e->buttons())
	{
	case Qt::LeftButton:
		break;
	case Qt::MiddleButton:
		lok[0] -= sqrt(viewRadius) * (u[0] * offsetX * 0.01 + v[0] * offsetY * 0.01);
		lok[1] -= sqrt(viewRadius) * (u[1] * offsetX * 0.01 + v[1] * offsetY * 0.01);
		lok[2] -= sqrt(viewRadius) * (u[2] * offsetX * 0.01 + v[2] * offsetY * 0.01);
		break;
	case Qt::RightButton:
		viewTheta += offsetY * 0.01;
		viewPhi += offsetX * 0.01;
		if (viewPhi > 2 * M_PI) viewPhi -= 2 * M_PI;
		else if (viewPhi < 2 * M_PI) viewPhi += 2 * M_PI;
		break;
	}
	
	oldPosX = e->x();
	oldPosY = e->y();
	fixView();
	update();
}

void ViewWidget::mouseReleaseEvent(QMouseEvent *e)
{

}

void ViewWidget::keyPressEvent(QKeyEvent *e)
{
	int key;

	key = e->key();

	printf("%c _ %d\n", key, key);
	update();
}

void ViewWidget::wheelEvent(QWheelEvent *e)
{
	viewRadius -= viewRadius * e->delta() / 250;
	fixView();
	update();
}


// RenderQuad() Renders a 1x1 quad in NDC, best used for framebuffer color targets
// and post-processing effects.

void ViewWidget::renderQuad()
{
	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void ViewWidget::makeModelVBO(vobj_t* vo)
{
	if (vo->numberOfChild == 1) {
		cout << "Make " << vo->dataItem->voxlData->name << " VBO." << endl;
		addVertexBufferObject(createVoxelVBO(vo), vboa);
		setColorVBO(1.2, 1.0, 0.47, 1.0, vboa->vbos[vboa->numVBOs - 1]);
		bindData(vboa->vbos[vboa->numVBOs - 1]);
		return;
	}
	else {
		vobj_t* voxelObjectPtr = vo->firstChild;
		while (voxelObjectPtr != NULL) {
			makeModelVBO(voxelObjectPtr);
			voxelObjectPtr = voxelObjectPtr->nextSibling;
		}
	}
}

//Private slots:
void ViewWidget::getVModelPtr(vmodel_t* vmodel) {
	this->vmodel = new VoxelModel;
	this->vmodel = vmodel;
	cout << this->vmodel->name << endl;
	makeModelVBO(this->vmodel->rootObj);
	update();
	return;
}
