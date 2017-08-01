#define _USE_MATH_DEFINES
#include "editwidget.h"
#include <iostream>
#include <math.h>
#include <time.h>
#include "LoadShaders.h"
#include "mymath.h"

#define THREE_DIMENSION_WINDOW 0
#define X_WINDOW 1
#define Y_WINDOW 2
#define Z_WINDOW 3
#define FOUR_WINDOWS 4

using namespace std;

enum RENDER_MODE { RENDER_POLYGON, RENDER_WIREFRAME, RENDER_PICKING };

EditWidget::EditWidget(QWidget * parent) : QOpenGLWidget(parent)
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
	left = -25;
	right = 25;
	bottom = -25;
	top = 25;
	nearClip = 0.1;
	farClip = 1000.0;
	fixView();

	sceneID = 0;

	windowmodeID = THREE_DIMENSION_WINDOW;

	// light
	setGLitAmbient(&globalLight, 0.3, 0.3, 0.3, 1.0);
	setLitPosition(&light0, 1, 1, 1, 0);
	setLitDirection(&light0, 1, 1, 1);
	setLitDiffuse(&light0, 0.4, 0.4, 0.4, 1.0);
	setLitSpecular(&light0, 0.45, 0.45, 0.45, 1.0);


	vboa = newVertexBufferObjectArray();

	//  set opengl version and profile
	//setupOpenGL();

}

EditWidget::~EditWidget()
{

}

void EditWidget::setupOpenGL(void)
{
	QSurfaceFormat format = QSurfaceFormat::defaultFormat();

	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setVersion(4, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	setFocusPolicy(Qt::StrongFocus);
}

void EditWidget::initializeGL(void)
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


	// enable clip plane
	glEnable(GL_CLIP_DISTANCE0);
}

void EditWidget::renderScene(int times)
{
	int i;

	for (i = 0; i < vboa->numVBOs; ++i)
	{
		if (i == 0) {
			glDisable(GL_CLIP_DISTANCE0);
		}
		drawData(vboa->vbos[i], RENDER_POLYGON, times);
		if (i == 0) {
			glEnable(GL_CLIP_DISTANCE0);
		}
	}
}

void EditWidget::renderBbox(int times)
{
	int i;

	for (i = 0; i < vboa->numVBOs; ++i)
	{
		if (vboa->vbos[i]->bboxDetect) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_LINE_SMOOTH);
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			drawBbox(vboa->vbos[i], RENDER_WIREFRAME, times);
			glDisable(GL_BLEND);
		}
	}
}

void EditWidget::setViewingMatrix(void)
{
	float mat[16], tmp[16], aspect;

	// initialize variables and assign them to shader program
	// viewing matrices
	// z-axis up

	identifyMat4fv(modelMat);						// model matrix
	transposeMat4fv(modelMat, mat);
	glUniformMatrix4fv(0, 1, false, mat);
	lookAt(eye, lok, vup, perspective_viewMat);					// view matrix
	transposeMat4fv(perspective_viewMat, mat);
	glUniformMatrix4fv(1, 1, false, mat);
	multMat4fv(perspective_viewMat, modelMat, mvMat);			// modelview matrix
	transposeMat4fv(mvMat, mat);
	glUniformMatrix4fv(3, 1, false, mat);
	normalM(mvMat, normalMat);						// normal matrix
	transposeMat4fv(normalMat, mat);
	glUniformMatrix4fv(5, 1, false, mat);
	aspect = this->width() / (float)this->height();	// projection matrix
	perspective(fovy, aspect, nearClip, farClip, perspectiveMat);
	transposeMat4fv(perspectiveMat, mat);
	glUniformMatrix4fv(2, 1, false, mat);
	multMat4fv(perspectiveMat, perspective_viewMat, mvpMat);
	multMat4fv(mvpMat, modelMat, mvpMat);
	transposeMat4fv(mvpMat, mat);
	glUniformMatrix4fv(4, 1, false, mat);
	glViewport(0, 0, this->width(), this->height());
}

void EditWidget::setLighting(void)
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

void EditWidget::setMaterial(vbo_t obj)
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

void EditWidget::fixView(void)
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

	lookAt(eye, lok, vup, perspective_viewMat);
}


void EditWidget::updateViewing(int times)
{
	float mat[16];

	switch (windowmodeID) {
	case THREE_DIMENSION_WINDOW:
		multMat4fv(perspective_viewMat, modelMat, mvMat);
		normalM(mvMat, normalMat);
		multMat4fv(perspectiveMat, mvMat, mvpMat);

		transposeMat4fv(modelMat, mat);
		glUniformMatrix4fv(0, 1, false, mat);
		transposeMat4fv(perspective_viewMat, mat);
		glUniformMatrix4fv(1, 1, false, mat);
		transposeMat4fv(normalMat, mat);
		glUniformMatrix4fv(5, 1, false, mat);
		transposeMat4fv(mvMat, mat);
		glUniformMatrix4fv(3, 1, false, mat);
		transposeMat4fv(mvpMat, mat);
		glUniformMatrix4fv(4, 1, false, mat);
		break;
	case X_WINDOW:
		multMat4fv(x_ortho_viewMat, modelMat, mvMat);
		normalM(mvMat, normalMat);
		multMat4fv(x_orthoMat, mvMat, mvpMat);

		transposeMat4fv(modelMat, mat);
		glUniformMatrix4fv(0, 1, false, mat);
		transposeMat4fv(x_ortho_viewMat, mat);
		glUniformMatrix4fv(1, 1, false, mat);
		transposeMat4fv(normalMat, mat);
		glUniformMatrix4fv(5, 1, false, mat);
		transposeMat4fv(mvMat, mat);
		glUniformMatrix4fv(3, 1, false, mat);
		transposeMat4fv(mvpMat, mat);
		glUniformMatrix4fv(4, 1, false, mat);
		break;
	case Y_WINDOW:
		multMat4fv(y_ortho_viewMat, modelMat, mvMat);
		normalM(mvMat, normalMat);
		multMat4fv(y_orthoMat, mvMat, mvpMat);

		transposeMat4fv(modelMat, mat);
		glUniformMatrix4fv(0, 1, false, mat);
		transposeMat4fv(y_ortho_viewMat, mat);
		glUniformMatrix4fv(1, 1, false, mat);
		transposeMat4fv(normalMat, mat);
		glUniformMatrix4fv(5, 1, false, mat);
		transposeMat4fv(mvMat, mat);
		glUniformMatrix4fv(3, 1, false, mat);
		transposeMat4fv(mvpMat, mat);
		glUniformMatrix4fv(4, 1, false, mat);
		break;
	case Z_WINDOW:
		multMat4fv(z_ortho_viewMat, modelMat, mvMat);
		normalM(mvMat, normalMat);
		multMat4fv(z_orthoMat, mvMat, mvpMat);

		transposeMat4fv(modelMat, mat);
		glUniformMatrix4fv(0, 1, false, mat);
		transposeMat4fv(z_ortho_viewMat, mat);
		glUniformMatrix4fv(1, 1, false, mat);
		transposeMat4fv(normalMat, mat);
		glUniformMatrix4fv(5, 1, false, mat);
		transposeMat4fv(mvMat, mat);
		glUniformMatrix4fv(3, 1, false, mat);
		transposeMat4fv(mvpMat, mat);
		glUniformMatrix4fv(4, 1, false, mat);
		break;
	case FOUR_WINDOWS:
		switch (times) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void EditWidget::bindData(vbo_t* vbo)
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

void EditWidget::drawData(vbo_t* const vbo, int mode,int times)
{
	int i;
	float rotMat[16];

	copyMat4fv(vbo->modelMat, modelMat);

	if (mode == RENDER_PICKING) {
		if (vbo->enableBuffers[FACE]) {
			glUseProgram(program[2]);
			updateViewing(times);
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
		updateViewing(times);
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
		updateViewing(times);
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

void EditWidget::drawBbox(vbo_t* const vbo, int mode,int times)
{
	int i;
	float rotMat[16], color[4];

	copyMat4fv(vbo->modelMat, modelMat);

	// render bounding box
	if (mode == RENDER_POLYGON) {
		if (vbo->bbox->enableBuffers[VERTEX]) {
			glUseProgram(program[3]);
			updateViewing(times);
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
			updateViewing(times);
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
			updateViewing(times);
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

void EditWidget::gBufferRendering(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program[4]);
	renderScene(0);
	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
}

void EditWidget::paintGL(void)
{
	float mat[16], color[4];

	glUseProgram(program[0]);
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


	if (windowmodeID != FOUR_WINDOWS) {
		glViewport(0, 0, this->width()/2, this->height()/2);
		renderScene(0);
		renderBbox(0);
	}
	else {
		for (int i = 0; i < 4; i++) {
			if (i == 0) {
				glViewport(0, 0, this->width() / 3, this->height() / 3);
			}
			else if (i == 1) {
				glViewport(0, this->height() / 3, this->width() / 3, (this->height() * 2) / 3);
			}
			else if (i == 2) {
				glViewport(0, this->height() * 2 / 3, this->width() / 3, this->height());
			}
			else {
				glViewport(this->width() / 3, 0, this->width(), this->height());
			}
			renderScene(i);
			renderBbox(i);
		}
	}
	

	glBindFramebuffer(GL_READ_FRAMEBUFFER, multiSampleFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->defaultFramebufferObject());
	glDrawBuffer(GL_BACK);
	glBlitFramebuffer(0, 0, this->width(), this->height(), 0, 0, this->width(), this->height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

}

void EditWidget::resizeGL(int width, int height)
{
	float mat[16];

	glViewport(0, 0, width, height);
	float aspect = width / (float)height;
	perspective(fovy, aspect, nearClip, farClip, perspectiveMat);
	transposeMat4fv(perspectiveMat, mat);
	glUniformMatrix4fv(2, 1, false, mat);


	update();
}

void EditWidget::mousePressEvent(QMouseEvent *e)
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

void EditWidget::mouseMoveEvent(QMouseEvent *e)
{
	float u[3] = { perspective_viewMat[0], perspective_viewMat[1], perspective_viewMat[2] };
	float v[3] = { perspective_viewMat[4], perspective_viewMat[5], perspective_viewMat[6] };
	float w[3] = { perspective_viewMat[8], perspective_viewMat[9], perspective_viewMat[10] };
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

void EditWidget::mouseReleaseEvent(QMouseEvent *e)
{

}

void EditWidget::keyPressEvent(QKeyEvent *e)
{
	int key;

	key = e->key();

	printf("%c _ %d\n", key, key);
	update();
}

void EditWidget::wheelEvent(QWheelEvent *e)
{
	viewRadius -= viewRadius * e->delta() / 250;
	fixView();
	update();
}

void EditWidget::makevDataVBO(vdata_t* vd)
{
	addVertexBufferObject(createVoxelVBO(vd), vboa);
	setColorVBO(1.2, 1.0, 0.47, 1.0, vboa->vbos[vboa->numVBOs - 1]);
	bindData(vboa->vbos[vboa->numVBOs - 1]);
	cout << "Make " << vd->name << " VBO." << endl;
}

//Private slots:
void EditWidget::getVDataPtr(vdata_t* vdata) {
	this->vdata = new VoxelData;
	this->vdata = vdata;
	cout << this->vdata->name << endl;
	makevDataVBO(this->vdata);
	update();
	return;
}
