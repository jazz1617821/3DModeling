#pragma once
#include <GL/glew.h>
#include <QOpenGLWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QSlider>
#include <QSpinBox>
#include "light.h"
#include "voxel.h"

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
	void cameramMove(float* eyeVec,float* lokVec, float* viewMat, int offsetX, int offsetY);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void keyPressEvent(QKeyEvent *e);
	void wheelEvent(QWheelEvent *e);

private:
	void setupOpenGL(void);
	void fixView(void);
	void updateViewing(int);
	void make_view(int);
	void make_projection(int);

	void drawMarking(int mode);

	void drawObject(int mode);

	void caculateVoxelAmount(vdata_t * vd);
	void createVoxelVBO(vdata_t * vd);
	void makevDataVBO(vdata_t* vd);

	void Texture();

	void makeDataBuffer(vdata_t* vd);

	private slots:
	void getVDataPtr(vdata_t* vdata);
	void xSliderValuechange(int value);
	void ySliderValuechange(int value);
	void zSliderValuechange(int value);
	void xSpinboxValuechange(int value);
	void ySpinboxValuechange(int value);
	void zSpinboxValuechange(int value);

signals:
	

private:
	QSlider* x_layer_slider;
	QSlider* y_layer_slider;
	QSlider* z_layer_slider;

	QSpinBox* x_layer_spinbox;
	QSpinBox* y_layer_spinbox;
	QSpinBox* z_layer_spinbox;

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

	// shader program
	enum Program_IDs { Simple, Slice, NumShaders };
	GLuint program[NumShaders];
	GLuint currentProgram;

	// mouse controll
	int oldPosX;
	int oldPosY;
	int selectedPosX;
	int selectedPoxY;

	// slice variable
	int x_number_of_layers;
	int y_number_of_layers;
	int z_number_of_layers;

	float clipplane_1[4];
	float clipplane_2[4];

	// vbo setting
	
	enum VAO_IDs { Ground, Triangles, XBillBoard, YBillBoard, ZBillBoard, NumVAOs };
	GLuint VAOs[NumVAOs];

	enum Object_Attribs { Vertex, Normal, Texcoor, Color, Wireframe, NumAttribs };
	GLuint groundvbo[NumAttribs];
	GLuint objvbo[NumAttribs];

	//3 Billboard
	GLuint x_billboard[NumAttribs];
	GLuint y_billboard[NumAttribs];
	GLuint z_billboard[NumAttribs];

	float xbbVertex[4][3] = {//yz_plane vertex
		{ 0,0,0 },
		{ 0,0,256 },
		{ 0,256,256 },
		{ 0,256,0 }
	};
	float ybbVertex[4][3] = {//xz_plane vertex
		{ 0,0,0 },
		{ 0,0,256 },
		{ 256,0,256 },
		{ 256,0,0 }
	};
	float zbbVertex[4][3] = {//xy_plane vertex
		{ 0,0,0 },
		{ 256,0,0 },
		{ 256,256,0 },
		{ 0,256,0 }
	};
	float xbbTexCoor[4][3] = {//yz_plane texture coordinate
		{ 0,0,0 },
		{ 0,0,1 },
		{ 0,1,1 },
		{ 0,1,0 }
	};
	float ybbTexCoor[4][3] = {//xz_plane texture coordinate
		{ 0,0,0 },
		{ 0,0,1 },
		{ 1,0,1 },
		{ 1,0,0 }
	};
	float zbbTexCoor[4][3] = {//xy_plane texture coordinate
		{ 0,0,0 },
		{ 1,0,0 },
		{ 1,1,0 },
		{ 0,1,0 }
	};

	
	int voxelamount = 0;

	//DataBuffer & Texture
	unsigned char* DataBuffer;

	GLuint Data3DTexture;
};
