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

EditWidget::EditWidget(QWidget * parent) : QOpenGLWidget(parent)
{
	setupOpenGL();

	//perspective view matrix init
	perspective_eye[0] = 0; perspective_eye[1] = 0; perspective_eye[2] = 1;
	perspective_lok[0] = 0; perspective_lok[1] = 0; perspective_lok[2] = 0;
	perspective_vup[0] = 0, perspective_vup[1] = 1, perspective_vup[2] = 0;
	//x paralle view matrix init
	x_ortho_eye[0] = 400; x_ortho_eye[1] = 0; x_ortho_eye[2] = 0;
	x_ortho_lok[0] = 0; x_ortho_lok[1] = 0; x_ortho_lok[2] = 0;
	x_ortho_vup[0] = 0, x_ortho_vup[1] = 1, x_ortho_vup[2] = 0;
	//y paralle view matrix init
	y_ortho_eye[0] = 0; y_ortho_eye[1] = 400; y_ortho_eye[2] = 0;
	y_ortho_lok[0] = 0; y_ortho_lok[1] = 0; y_ortho_lok[2] = 0;
	y_ortho_vup[0] = 0, y_ortho_vup[1] = 0, y_ortho_vup[2] = 1;
	//z paralle view matrix init
	z_ortho_eye[0] = 0; z_ortho_eye[1] = 0; z_ortho_eye[2] = -400;
	z_ortho_lok[0] = 0; z_ortho_lok[1] = 0; z_ortho_lok[2] = 0;
	z_ortho_vup[0] = 0, z_ortho_vup[1] = 1, z_ortho_vup[2] = 0;

	viewTrans[0] = 0; viewTrans[1] = 0; viewTrans[2] = 0;
	viewRot[0] = 0; viewRot[1] = 0; viewRot[2] = 0;
	viewPhi = radians(0);
	viewTheta = radians(60);
	viewRadius = 400;
	fovy = 45.0;
	aspect = this->width()/(float)(this->height());
	x_ortho_width = 50;
	y_ortho_width = 50;
	z_ortho_width = 50;
	nearClip = 1;
	farClip = 1000.0;
	fixView();

	// slice variable
	x_number_of_layers = y_number_of_layers = z_number_of_layers = 0;

	windowmodeID = THREE_DIMENSION_WINDOW;
	vdata = NULL;


	//slider 
	
	x_layer_slider = new QSlider(Qt::Vertical, this);
	x_layer_slider->setRange(0, 256);
	x_layer_slider->setValue(0);
	x_number_of_layers = x_layer_slider->value();
	x_layer_slider->setHidden(true);
	
	y_layer_slider = new QSlider(Qt::Vertical, this);
	y_layer_slider->setRange(0, 256);
	y_layer_slider->setValue(0);
	y_number_of_layers = y_layer_slider->value();
	y_layer_slider->setHidden(true);

	z_layer_slider = new QSlider(Qt::Vertical, this);
	z_layer_slider->setRange(0, 256);
	z_layer_slider->setValue(0);
	z_number_of_layers = z_layer_slider->value();
	z_layer_slider->setHidden(true);
	
}

EditWidget::~EditWidget()
{

}

void EditWidget::setupOpenGL(void)
{
	QSurfaceFormat format = QSurfaceFormat::defaultFormat();

	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setVersion(4, 0);
	format.setProfile(QSurfaceFormat::CompatibilityProfile);
	QSurfaceFormat::setDefaultFormat(format);

	setFocusPolicy(Qt::StrongFocus);
}

void EditWidget::initializeGL(void)
{
	x_number_of_layers = x_layer_slider->value();
	y_number_of_layers = y_layer_slider->value();
	z_number_of_layers = z_layer_slider->value();

	float mat[16];

	//  initialize glew
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}


	GLfloat ground[256 * 4][3];

	for (int x = 0; x < 256; x++) {
		ground[x * 2][0] = x;
		ground[x * 2][1] = 0;
		ground[x * 2][2] = 0;
		ground[x * 2 + 1][0] = x;
		ground[x * 2 + 1][1] = 256;
		ground[x * 2 + 1][2] = 0;
	}

	for (int y = 0; y < 256; y++) {
		ground[y * 2 + 512][0] = 0;
		ground[y * 2 + 512][1] = y;
		ground[y * 2 + 512][2] = 0;
		ground[y * 2 + 1 + 512][0] = 256;
		ground[y * 2 + 1 + 512][1] = y;
		ground[y * 2 + 1 + 512][2] = 0;
	}

	/* Allocate and assign a Vertex Array Object to our handle */
	glGenVertexArrays(1, VAOs);

	/* Bind our Vertex Array Object as the current used object */
	glBindVertexArray(VAOs[Ground]);

	//Gen Buffers
	glGenBuffers(1, Ground_Vertex_Buffer);

	//Bind ground data
	glBindBuffer(GL_ARRAY_BUFFER, Ground_Vertex_Buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);

	// load shaders

	// simple shader info
	ShaderInfo simpleShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\simple.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\simple.frag" },
		{ GL_NONE, NULL }
	};

	// simple shader
	program[Simple] = LoadShaders(simpleShaders);

	// slice shader info
	ShaderInfo sliceShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\slice.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\slice.frag" },
		{ GL_NONE, NULL }
	};

	// slice shader
	program[Slice] = LoadShaders(sliceShaders);

	// use shader program
	//glUseProgram(program[Simple]);
	//currentProgram = program[Simple];

	glEnable(GL_DEPTH_TEST);
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
		perspective_vup[0] = 0;
		perspective_vup[1] = 1;
		perspective_vup[2] = 0;
	}

	perspective_eye[0] = perspective_lok[0] + viewRadius * cos(viewPhi) * sin(viewTheta);
	perspective_eye[1] = perspective_lok[1] + viewRadius * cos(viewTheta);
	perspective_eye[2] = perspective_lok[2] + viewRadius * sin(viewPhi) * sin(viewTheta);
}

void EditWidget::updateViewing(int mode)
{
	float mat[16];

	switch (mode) {
	case THREE_DIMENSION_WINDOW:
		multMat4fv(perspective_viewMat, modelMat, mvMat);
		multMat4fv(perspectiveMat, mvMat, mvpMat);

		transposeMat4fv(mvpMat, mat);
		glUniformMatrix4fv(glGetUniformLocation(currentProgram, "modelMat"), 1, false, modelMat);
		glUniformMatrix4fv(glGetUniformLocation(currentProgram, "mvpMat"), 1, false, mat);
		break;
	case X_WINDOW:
		multMat4fv(x_ortho_viewMat, modelMat, mvMat);
		multMat4fv(x_orthoMat, mvMat, mvpMat);

		transposeMat4fv(mvpMat, mat);
		glUniformMatrix4fv(glGetUniformLocation(currentProgram, "modelMat"), 1, false, modelMat);
		glUniformMatrix4fv(glGetUniformLocation(currentProgram, "mvpMat"), 1, false, mat);
		break;
	case Y_WINDOW:
		multMat4fv(y_ortho_viewMat, modelMat, mvMat);
		multMat4fv(y_orthoMat, mvMat, mvpMat);

		transposeMat4fv(mvpMat, mat);
		glUniformMatrix4fv(glGetUniformLocation(currentProgram, "modelMat"), 1, false, modelMat);
		glUniformMatrix4fv(glGetUniformLocation(currentProgram, "mvpMat"), 1, false, mat);
		break;
	case Z_WINDOW:
		multMat4fv(z_ortho_viewMat, modelMat, mvMat);
		multMat4fv(z_orthoMat, mvMat, mvpMat);

		transposeMat4fv(mvpMat, mat);
		glUniformMatrix4fv(glGetUniformLocation(currentProgram, "modelMat"), 1, false, modelMat);
		glUniformMatrix4fv(glGetUniformLocation(currentProgram, "mvpMat"), 1, false, mat);
		break;
	}

}

void EditWidget::make_view(int mode) {
	switch (mode) {
	case THREE_DIMENSION_WINDOW:	 /* Perspective */
		lookAt(perspective_eye, perspective_lok, perspective_vup, perspective_viewMat);
		break;
	case X_WINDOW:       /* X direction parallel viewing */
		lookAt(x_ortho_eye, x_ortho_lok, x_ortho_vup, x_ortho_viewMat);
		break;
	case Y_WINDOW:       /* Y direction parallel viewing */
		lookAt(y_ortho_eye, y_ortho_lok, y_ortho_vup, y_ortho_viewMat);
		break;
	case Z_WINDOW:
		lookAt(z_ortho_eye, z_ortho_lok, z_ortho_vup, z_ortho_viewMat);
		break;
	}
}

void EditWidget::make_projection(int mode) {
	switch (mode) {
	case THREE_DIMENSION_WINDOW:	 /* Perspective projection*/
		perspective(fovy, aspect, nearClip, farClip, perspectiveMat);
		break;
	case X_WINDOW:       /* X direction parallel projection */
		ortho(-x_ortho_width / 2, x_ortho_width / 2, -x_ortho_width / (2 * aspect), x_ortho_width / (2 * aspect), nearClip, farClip, x_orthoMat);
		break;
	case Y_WINDOW:       /* Y direction parallel projection */
		ortho(-y_ortho_width / 2, y_ortho_width / 2, -y_ortho_width / (2 * aspect), y_ortho_width / (2 * aspect), nearClip, farClip, y_orthoMat);
		break;
	case Z_WINDOW:		/* Z direction parallel projection */
		ortho(-z_ortho_width / 2, z_ortho_width / 2, -z_ortho_width / (2 * aspect), z_ortho_width / (2 * aspect), nearClip, farClip, z_orthoMat);
		break;
	}
}

void EditWidget::drawMarking(int mode) {
	float mat[16], color[4];
	glUseProgram(program[Simple]);
	currentProgram = program[Simple];

	switch (mode) {
	case X_WINDOW:
		translate(-256 / 2, -256 / 2, 0, modelMat);
		rotateY(90, mat);
		multMat4fv(mat, modelMat, modelMat);
		updateViewing(X_WINDOW);
		color[0] = 1.0;
		color[1] = 0.0;
		color[2] = 0.0;
		color[3] = 1.0;
		break;
	case Y_WINDOW:
		translate(-256 / 2, -256 / 2, 0, modelMat);
		rotateX(90, mat);
		multMat4fv(mat, modelMat, modelMat);
		updateViewing(Y_WINDOW);
		color[0] = 0.0;
		color[1] = 1.0;
		color[2] = 0.0;
		color[3] = 1.0;
		break;
	case Z_WINDOW:
		translate(-256 / 2, -256 / 2, 0, modelMat);
		updateViewing(Z_WINDOW);
		color[0] = 0.0;
		color[1] = 0.0;
		color[2] = 1.0;
		color[3] = 1.0;
		break;

	case THREE_DIMENSION_WINDOW:
		translate(-256 / 2, -256 / 2, 0, modelMat);
		identifyMat4fv(mat);
		rotateX(90, mat);
		multMat4fv(mat, modelMat, modelMat);
		updateViewing(THREE_DIMENSION_WINDOW);
		color[0] = 0.0;
		color[1] = 0.0;
		color[2] = 0.0;
		color[3] = 1.0;
		break;
	default:
		break;
	}
	glUniform4fv(glGetUniformLocation(currentProgram, "color"), 1, color);

	//glBindVertexArray(VAOs[Ground]);
	glBindBuffer(GL_ARRAY_BUFFER, Ground_Vertex_Buffer[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINES, 0, 256 * 4);
	//glBindVertexArray(0);
}

void EditWidget::drawObject(int mode) {
	float mat[16], color[4];

	switch (mode) {
	case X_WINDOW:
		glUseProgram(program[Slice]);
		currentProgram = program[Slice];
		translate(-256 / 2, -256 / 2, -256 / 2, modelMat);
		updateViewing(X_WINDOW);

		clipplane_1[1] = clipplane_1[2] = clipplane_2[1] = clipplane_2[2] = 0;
		clipplane_1[0] = 1;
		clipplane_2[0] = -1;
		clipplane_1[3] = x_number_of_layers;
		clipplane_2[3] = x_number_of_layers;

		glUniform4fv(glGetUniformLocation(currentProgram, "ClipPlaneUp"), 1, clipplane_1);
		glUniform4fv(glGetUniformLocation(currentProgram, "ClipPlaneDoen"), 1, clipplane_2);

		color[0] = 1.0;
		color[1] = 1.0;
		color[2] = 1.0;
		color[3] = 1.0;
		glUniform4fv(glGetUniformLocation(currentProgram, "color"), 1, color);
		glBindVertexArray(VAOs[Triangles]);
		glBindBuffer(GL_ARRAY_BUFFER, Triangles_Vertex_Buffer[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(float) * 108 * voxelamount);

		color[0] = 0.0;
		color[1] = 0.0;
		color[2] = 0.0;
		color[3] = 1.0;
		glUniform4fv(glGetUniformLocation(currentProgram, "color"), 1, color);
		glBindVertexArray(VAOs[Wireframe]);
		glBindBuffer(GL_ARRAY_BUFFER, Wireframe_Vertex_Buffer[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_LINES, 0, sizeof(float) * 144 * voxelamount);
		break;
	case Y_WINDOW:
		glUseProgram(program[Slice]);
		currentProgram = program[Slice];
		translate(-256 / 2, -256 / 2, -256 / 2, modelMat);
		updateViewing(Y_WINDOW);

		clipplane_1[0] = clipplane_1[2] = clipplane_2[0] = clipplane_2[2] = 0;
		clipplane_1[1] = 1;
		clipplane_2[1] = -1;
		clipplane_1[3] = y_number_of_layers;
		clipplane_2[3] = y_number_of_layers;

		glUniform4fv(glGetUniformLocation(currentProgram, "ClipPlaneUp"), 1, clipplane_1);
		glUniform4fv(glGetUniformLocation(currentProgram, "ClipPlaneDoen"), 1, clipplane_2);

		color[0] = 1.0;
		color[1] = 1.0;
		color[2] = 1.0;
		color[3] = 1.0;
		glUniform4fv(glGetUniformLocation(currentProgram, "color"), 1, color);
		glBindVertexArray(VAOs[Triangles]);
		glBindBuffer(GL_ARRAY_BUFFER, Triangles_Vertex_Buffer[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(float) * 108 * voxelamount);

		color[0] = 0.0;
		color[1] = 0.0;
		color[2] = 0.0;
		color[3] = 1.0;
		glUniform4fv(glGetUniformLocation(currentProgram, "color"), 1, color);
		glBindVertexArray(VAOs[Wireframe]);
		glBindBuffer(GL_ARRAY_BUFFER, Wireframe_Vertex_Buffer[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_LINES, 0, sizeof(float) * 144 * voxelamount);
		break;
	case Z_WINDOW:
		glUseProgram(program[Slice]);
		currentProgram = program[Slice];
		translate(-256 / 2, -256 / 2, -256 / 2, modelMat);
		updateViewing(Z_WINDOW);

		clipplane_1[0] = clipplane_1[1] = clipplane_2[0] = clipplane_2[1] = 0;
		clipplane_1[2] = 1;
		clipplane_2[2] = -1;
		clipplane_1[3] = z_number_of_layers;
		clipplane_2[3] = z_number_of_layers;

		glUniform4fv(glGetUniformLocation(currentProgram, "ClipPlaneUp"), 1, clipplane_1);
		glUniform4fv(glGetUniformLocation(currentProgram, "ClipPlaneDoen"), 1, clipplane_2);

		color[0] = 1.0;
		color[1] = 1.0;
		color[2] = 1.0;
		color[3] = 1.0;
		glUniform4fv(glGetUniformLocation(currentProgram, "color"), 1, color);
		glBindVertexArray(VAOs[Triangles]);
		glBindBuffer(GL_ARRAY_BUFFER, Triangles_Vertex_Buffer[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(float) * 108 * voxelamount);

		color[0] = 0.0;
		color[1] = 0.0;
		color[2] = 0.0;
		color[3] = 1.0;
		glUniform4fv(glGetUniformLocation(currentProgram, "color"), 1, color);
		glBindVertexArray(VAOs[Wireframe]);
		glBindBuffer(GL_ARRAY_BUFFER, Wireframe_Vertex_Buffer[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_LINES, 0, sizeof(float) * 144 * voxelamount);

		break;

	case THREE_DIMENSION_WINDOW:
		glUseProgram(program[Simple]);
		currentProgram = program[Simple];
		translate(-256 / 2, 0, -256 / 2, modelMat);
		updateViewing(THREE_DIMENSION_WINDOW);

		color[0] = 1.0;
		color[1] = 1.0;
		color[2] = 1.0;
		color[3] = 1.0;
		glUniform4fv(glGetUniformLocation(currentProgram,"color"), 1, color);
		glBindVertexArray(VAOs[Triangles]);
		glBindBuffer(GL_ARRAY_BUFFER, Triangles_Vertex_Buffer[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(float) * 108 * voxelamount);

		/*
		color[0] = 0.0;
		color[1] = 0.0;
		color[2] = 0.0;
		color[3] = 1.0;
		glUniform4fv(glGetUniformLocation(currentProgram, "color"), 1, color);
		glBindVertexArray(VAOs[Wireframe]);
		glBindBuffer(GL_ARRAY_BUFFER, Wireframe_Vertex_Buffer[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_LINES, 0, sizeof(float) * 144 * voxelamount);
		*/

		break;
	default:
		break;
	}
}

void EditWidget::paintGL(void)
{

	glClearColor(0.66, 0.66, 0.66, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_BACK);

	switch (windowmodeID) {
	case FOUR_WINDOWS:
		x_layer_slider->setHidden(false);
		//x_layer_slider->setGeometry();
		make_view(X_WINDOW);
		make_projection(X_WINDOW);
		glViewport(0, 0, this->width() / 2, this->height() / 2);
		drawMarking(X_WINDOW);
		if (vdata != NULL) {
			drawObject(X_WINDOW);
		}

		make_view(Y_WINDOW);
		make_projection(Y_WINDOW);
		glViewport(0, this->height() / 2, this->width() / 2, this->height() / 2);
		drawMarking(Y_WINDOW);
		if (vdata != NULL) {
			drawObject(Y_WINDOW);
		}

		make_view(Z_WINDOW);
		make_projection(Z_WINDOW);
		glViewport(this->width() / 2, 0, this->width() / 2, this->height() / 2);
		drawMarking(Z_WINDOW);
		if (vdata != NULL) {
			drawObject(Z_WINDOW);
		}

		make_view(THREE_DIMENSION_WINDOW);
		make_projection(THREE_DIMENSION_WINDOW);
		glViewport(this->width() / 2, this->height() / 2, this->width() / 2, this->height() / 2);
		drawMarking(THREE_DIMENSION_WINDOW);
		if (vdata != NULL) {
			drawObject(THREE_DIMENSION_WINDOW);
		}

		break;

	case X_WINDOW:
		make_view(X_WINDOW);
		make_projection(X_WINDOW);
		glViewport(0, 0, this->width(), this->height());
		drawMarking(X_WINDOW);
		if (vdata != NULL) {
			glUseProgram(program[1]);
			drawObject(X_WINDOW);
		}

		break;
	case Y_WINDOW:
		make_view(Y_WINDOW);
		make_projection(Y_WINDOW);
		glViewport(0, 0, this->width(), this->height());
		drawMarking(Y_WINDOW);
		if (vdata != NULL) {
			drawObject(Y_WINDOW);
		}

		break;
	case Z_WINDOW:
		make_view(Z_WINDOW);
		make_projection(Z_WINDOW);
		glViewport(0, 0, this->width(), this->height());
		drawMarking(Z_WINDOW);
		if (vdata != NULL) {
			drawObject(Z_WINDOW);
		}

		break;
	case THREE_DIMENSION_WINDOW:
		make_view(THREE_DIMENSION_WINDOW);
		make_projection(THREE_DIMENSION_WINDOW);
		glViewport(0, 0, this->width(), this->height());
		drawMarking(THREE_DIMENSION_WINDOW);
		if (vdata != NULL) {
			drawObject(THREE_DIMENSION_WINDOW);
		}
		break;
	}
	
}

void EditWidget::resizeGL(int width, int height)
{
	float mat[16];

	aspect = width / (float)height;

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

void EditWidget::cameramMove(float* eyeVec,float * lokVec, float * viewMat, int offsetX, int offsetY)
{
	float u[3] = { viewMat[0], viewMat[1], viewMat[2] };
	float v[3] = { viewMat[4], viewMat[5], viewMat[6] };
	float w[3] = { viewMat[8], viewMat[9], viewMat[10] };

	lokVec[0] += sqrt(viewRadius) * (u[0] * offsetX * 0.01 + v[0] * offsetY * 0.01);
	lokVec[1] += sqrt(viewRadius) * (u[1] * offsetX * 0.01 + v[1] * offsetY * 0.01);
	lokVec[2] += sqrt(viewRadius) * (u[2] * offsetX * 0.01 + v[2] * offsetY * 0.01);

	eyeVec[0] += sqrt(viewRadius) * (u[0] * offsetX * 0.01 + v[0] * offsetY * 0.01);
	eyeVec[1] += sqrt(viewRadius) * (u[1] * offsetX * 0.01 + v[1] * offsetY * 0.01);
	eyeVec[2] += sqrt(viewRadius) * (u[2] * offsetX * 0.01 + v[2] * offsetY * 0.01);
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
		switch (windowmodeID) {
		case FOUR_WINDOWS:
		if (e->x() < this->width() / 2 && e->y() > this->height() / 2) {		//mouse in x ortho window
			cameramMove(x_ortho_eye, x_ortho_lok, x_ortho_viewMat, offsetX, offsetY);
		}
		else if (e->x() < this->width() / 2 && e->y() < this->height() / 2) {	//mouse in y ortho window
			cameramMove(y_ortho_eye, y_ortho_lok, y_ortho_viewMat, offsetX, offsetY);
		}
		else if (e->x() > this->width() / 2 && e->y() > this->height() / 2) {	//mouse in z ortho window
			cameramMove(z_ortho_eye, z_ortho_lok, z_ortho_viewMat, offsetX, offsetY);
		}
		else if (e->x() > this->width() / 2 && e->y() < this->height() / 2) {		//mouse in three dimension window
			perspective_lok[0] -= sqrt(viewRadius) * (u[0] * offsetX * 0.01 + v[0] * offsetY * 0.01);
			perspective_lok[1] -= sqrt(viewRadius) * (u[1] * offsetX * 0.01 + v[1] * offsetY * 0.01);
			perspective_lok[2] -= sqrt(viewRadius) * (u[2] * offsetX * 0.01 + v[2] * offsetY * 0.01);
		}
		break;
		case THREE_DIMENSION_WINDOW:
			perspective_lok[0] -= sqrt(viewRadius) * (u[0] * offsetX * 0.01 + v[0] * offsetY * 0.01);
			perspective_lok[1] -= sqrt(viewRadius) * (u[1] * offsetX * 0.01 + v[1] * offsetY * 0.01);
			perspective_lok[2] -= sqrt(viewRadius) * (u[2] * offsetX * 0.01 + v[2] * offsetY * 0.01);
		break;
		case X_WINDOW:
			cameramMove(x_ortho_eye, x_ortho_lok, x_ortho_viewMat, offsetX, offsetY);
		break;
		case Y_WINDOW:
			cameramMove(y_ortho_eye, y_ortho_lok, y_ortho_viewMat, offsetX, offsetY);
		break;
		case Z_WINDOW:
			cameramMove(z_ortho_eye, z_ortho_lok, z_ortho_viewMat, offsetX, offsetY);
		break;
		default:
		break;
		}
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

	if (e->key() == '0') {
		windowmodeID = FOUR_WINDOWS;
		cout << "FOUR_WINDOWS" << endl;
	}
	else if (e->key() == '1') {
		windowmodeID = X_WINDOW;
		cout << "X_WINDOW" << endl;
	}
	else if (e->key() == '3') {
		windowmodeID = Z_WINDOW;
		cout << "Z_WINDOW" << endl;
	}
	else if (e->key() == '7') {
		windowmodeID = Y_WINDOW;
		cout << "Y_WINDOW" << endl;
	}
	else if (e->key() == '9') {
		windowmodeID = THREE_DIMENSION_WINDOW;
		cout << "THREE_DIMENSION_WINDOW" << endl;
	}


	printf("%c _ %d\n", key, key);
	update();
}

void EditWidget::wheelEvent(QWheelEvent *e)
{
	switch (windowmodeID) {
	case FOUR_WINDOWS:
		if (e->x() < this->width() / 2 && e->y() > this->height() / 2) {		//mouse in x ortho window
			x_ortho_width -= x_ortho_width * e->delta() / 250;
		}
		else if (e->x() < this->width() / 2 && e->y() < this->height() / 2) {	//mouse in y ortho window
			y_ortho_width -= y_ortho_width * e->delta() / 250;
		}
		else if (e->x() > this->width() / 2 && e->y() > this->height() / 2) {	//mouse in z ortho window
			z_ortho_width -= z_ortho_width * e->delta() / 250;
		}
		else if (e->x() > this->width() / 2 && e->y() < this->height() / 2) {	//mouse in three dimension window
			viewRadius -= viewRadius * e->delta() / 250;
			fixView();
		}
		break;
	case THREE_DIMENSION_WINDOW:
		viewRadius -= viewRadius * e->delta() / 250;
		fixView();
		break;
	case X_WINDOW:
			
		break;
	case Y_WINDOW:
			
		break;
	case Z_WINDOW:
			
		break;
	default:
		break;
	}
	update();
}

void EditWidget::caculateVoxelAmount(vdata_t* vd) {
	int amount = 0;

	for (int i = 0; i < vd->resolution[0] * vd->resolution[1] * vd->resolution[2]; i++) {
		if (vd->rawData[i].data != 0) {
			amount++;
		}
	}
	voxelamount = amount;
}

void EditWidget::makevDataVBO(vdata_t* vd)
{
	static bool triangleflag = false;
	static bool wireframeflag = false;
	float cubeboxpoint[8][3] = {
		{ 0,0,0 },
		{ 1,0,0 },
		{ 1,0,1 },
		{ 0,0,1 },
		{ 0,1,0 },
		{ 1,1,0 },
		{ 1,1,1 },
		{ 0,1,1 }
	};

	int triangle_index[12][3] = {
		{ 0,1,4 },
		{ 1,5,4 },
		{ 1,2,5 },
		{ 2,6,5 },
		{ 2,3,6 },
		{ 3,7,6 },
		{ 3,0,7 },
		{ 0,4,7 },
		{ 4,5,7 },
		{ 5,6,7 },
		{ 3,2,0 },
		{ 2,1,0 }
	};

	int wireframe_index[24][2] = {
		//| -
		{ 0,4 },
		{ 1,5 },
		{ 2,6 },
		{ 3,7 },
		{ 0,1 },
		{ 1,2 },
		{ 2,3 },
		{ 3,0 },
		{ 4,5 },
		{ 5,6 },
		{ 6,7 },
		{ 7,4 },
		//X
		{ 0,5 },
		{ 1,4 },
		{ 1,6 },
		{ 2,5 },
		{ 2,7 },
		{ 3,6 },
		{ 3,4 },
		{ 0,7 },
		{ 4,6 },
		{ 5,7 },
		{ 0,2 },
		{ 1,3 }
	};

	cout << "Make " << vd->name << "full Object Triangle and Wireframe vertex buffer." << endl;
	
	float* triangle_buffer = (float*)malloc(sizeof(float) * 108 * voxelamount);
	float* wireframe_buffer = (float*)malloc(sizeof(float) * 144 * voxelamount);

	int count = 0;
	for (int z = 0; z < vd->resolution[2]; z++) {
		for (int y = 0; y < vd->resolution[1]; y++) {
			for (int x = 0; x < vd->resolution[0]; x++) {
				if (vd->rawData[x+ y * vd->resolution[0] + z * vd->resolution[0] * vd->resolution[1]].data != 0) {
					//Triangle vertex 
					int flag = 0;
					if (x == 0 || vd->rawData[x - 1 + y * vd->resolution[0] + z * vd->resolution[0] * vd->resolution[1]].data == 0 || x == vd->resolution[0] - 1 || vd->rawData[x + 1 + y * vd->resolution[0] + z * vd->resolution[0] * vd->resolution[1]].data == 0) {
						flag = 1;
					}
					if (y == 0 || vd->rawData[x + (y - 1) * vd->resolution[0] + z * vd->resolution[0] * vd->resolution[1]].data == 0 || y == vd->resolution[1] - 1 || vd->rawData[x + (y + 1) * vd->resolution[0] + z * vd->resolution[0] * vd->resolution[1]].data == 0) {
						flag = 1;
					}
					if (z == 0 || vd->rawData[x + y * vd->resolution[0] + (z - 1) * vd->resolution[0] * vd->resolution[1]].data == 0 || z == vd->resolution[2] - 1 || vd->rawData[x + y * vd->resolution[0] + (z + 1) * vd->resolution[0] * vd->resolution[1]].data == 0) {
						flag = 1;
					}
					if (flag) {
						for (int i = 0; i < 12; i++) {
							for (int j = 0; j < 3; j++) {
								triangle_buffer[count * 108 + i * 9 + j * 3 + 0] = x + cubeboxpoint[triangle_index[i][j]][0];
								triangle_buffer[count * 108 + i * 9 + j * 3 + 1] = y + cubeboxpoint[triangle_index[i][j]][1];
								triangle_buffer[count * 108 + i * 9 + j * 3 + 2] = z + cubeboxpoint[triangle_index[i][j]][2];
							}
						}
						//Wireframe vertex
						for (int i = 0; i < 24; i++) {
							for (int j = 0; j < 2; j++) {
								wireframe_buffer[count * 108 + i * 6 + j * 3 + 0] = x + cubeboxpoint[wireframe_index[i][j]][0];
								wireframe_buffer[count * 108 + i * 6 + j * 3 + 1] = y + cubeboxpoint[wireframe_index[i][j]][1];
								wireframe_buffer[count * 108 + i * 6 + j * 3 + 2] = z + cubeboxpoint[wireframe_index[i][j]][2];
							}
						}
						count++;
					}
				}
			}
		}
	}
	cout << count << endl;
	//Triangles

	//Gen Buffers
	if (!triangleflag) {
		/* Allocate and assign a Vertex Array Object to our handle */
		glGenVertexArrays(1, VAOs);

		/* Bind our Vertex Array Object as the current used object */
		glBindVertexArray(VAOs[Triangles]);

		glGenBuffers(1, Triangles_Vertex_Buffer);
		triangleflag = true;
	}

	//Bind triangles data
	glBindBuffer(GL_ARRAY_BUFFER, Triangles_Vertex_Buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108 * voxelamount, triangle_buffer, GL_STATIC_DRAW);

	free(triangle_buffer);


	//Wireframe

	//Gen Buffers
	if (!wireframeflag) {
		/* Allocate and assign a Vertex Array Object to our handle */
		glGenVertexArrays(1, VAOs);

		/* Bind our Vertex Array Object as the current used object */
		glBindVertexArray(VAOs[Wireframe]);

		glGenBuffers(1, Wireframe_Vertex_Buffer);
		wireframeflag = true;
	}

	//Bind triangles data
	glBindBuffer(GL_ARRAY_BUFFER, Wireframe_Vertex_Buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108 * voxelamount, wireframe_buffer, GL_STATIC_DRAW);

	free(wireframe_buffer);


	return;
}

//Private slots:
void EditWidget::getVDataPtr(vdata_t* vdata) {
	if (this->vdata == NULL) {
		this->vdata = new VoxelData;
	}
	this->vdata = vdata;
	cout << this->vdata->name << endl;
	caculateVoxelAmount(this->vdata);
	makevDataVBO(this->vdata);
	update();
	return;
}