#pragma once
#include <GL/glew.h>
#include <QOpenGLWidget>
#include <QPushButton>
#include <QMouseEvent>
#include "object.h"
#include "light.h"
#include "voxel.h"
#include "voxelvbo.h"

class EditWidget : public QOpenGLWidget {
	Q_OBJECT

public:
	EditWidget(QWidget* parent = Q_NULLPTR);
	~EditWidget();

protected:
	void initializeGL(void);
	void paintGL(void);
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void keyPressEvent(QKeyEvent *e);
	void wheelEvent(QWheelEvent *e);

private:
	void setupOpenGL(void);
	void setViewingMatrix(void);
	void fixView(void);
	void updateViewing(int);
	void bindData(vbo_t* pObj);
	void make_view(int);
	void make_projection(int);
	void drawVBO();

	//make VBO
	void makevDataVBO(vdata_t*);

	private slots:
	void getVDataPtr(vdata_t* vdata);

signals:


private:
	//Voxel Data
	VoxelData* vdata;
	int numFacesVoxel;

private:

	// camera
	float perspective_eye[3];
	float perspective_lok[3];
	float perspective_vup[3];
	float x_ortho_eye[3];
	float x_ortho_lok[3];
	float x_ortho_vup[3];
	float y_ortho_eye[3];
	float y_ortho_lok[3];
	float y_ortho_vup[3];
	float z_ortho_eye[3];
	float z_ortho_lok[3];
	float z_ortho_vup[3];
	float u[3];
	float v[3];
	float w[3];
	float viewTrans[3];
	float viewRot[3];
	float viewPhi;
	float viewTheta;
	float viewRadius;
	float fovy;
	float aspect;
	float x_ortho_width;
	float y_ortho_width;
	float z_ortho_width;
	float nearClip;
	float farClip;
	float curRot[16];
	float lastRot[16];

	// scene id
	int sceneID;

	//windows mode id
	int windowmodeID;

	// lights
	lit_t light0;
	glit_t globalLight;

	// matrices
	float modelMat[16];
	float perspective_viewMat[16];
	float x_ortho_viewMat[16];
	float y_ortho_viewMat[16];
	float z_ortho_viewMat[16];
	float perspectiveMat[16];
	float x_orthoMat[16];
	float y_orthoMat[16];
	float z_orthoMat[16];
	float mvMat[16];
	float mvpMat[16];
	float normalMat[16];

	float lightSpaceMat[16];

	// object uniform location
	GLuint colorLoc;

	// shader program
	GLuint program[1];

	// mouse controll
	int oldPosX;
	int oldPosY;
	int selectedPosX;
	int selectedPoxY;

	GLuint triangleVBO;
	GLuint lineVBO;
};
