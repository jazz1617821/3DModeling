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
	void setLighting(void);
	void setMaterial(vbo_t obj);
	void fixView(void);
	void updateViewing(int);
	void bindData(vbo_t* pObj);
	void drawData(vbo_t* const pObj, int mode,int times);
	void drawBbox(vbo_t* const pObj, int mode,int times);
	void renderScene(int);
	void renderBbox(int);

	// shadow mapping
	void genShadowMapping(void);
	void configureShadowMapping(void);
	void renderQuad(void);

	// multi-sampling
	void genMultiSampling(void);

	// ssao
	void genSSAO(void);
	void resizeSSAO(void);
	void configureGBufferShader(void);
	void gBufferRendering(void);
	void ssaoTextureRendering(void);
	void ssaoBlurRendering(void);
	void ssaoLightPassRendering(void);
	void shadowMappingRendering(void);

	//make VBO
	void makevDataVBO(vdata_t*);

	private slots:
	void getVDataPtr(vdata_t* vdata);

signals:


private:
	//Voxel Model
	VoxelData* vdata;

private:
	int iconSize;

	// camera
	float eye[3];
	float lok[3];
	float vup[3];
	float u[3];
	float v[3];
	float w[3];
	float viewTrans[3];
	float viewRot[3];
	float viewPhi;
	float viewTheta;
	float viewRadius;
	float fovy;
	float left;
	float right;
	float bottom;
	float top;
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

	// shadow
	GLuint depthMapFBO;
	GLuint shadowWidth;
	GLuint shadowHeight;
	GLuint depthMap;

	// multi-sampling
	GLuint multiSampleTex;
	GLuint multiSampleFBO;
	GLuint multiSampleColorBuffer;
	GLuint multiSampleDepthBuffer;

	// ssao
	GLuint kernelSize;
	GLuint noiseSize;
	GLfloat kernelRadius;
	GLfloat *ssaoKernel;
	GLfloat *ssaoNoise;
	GLuint quadVAO;
	GLuint quadVBO;

	GLuint gBuffer;
	GLuint gPositionDepth;
	GLuint gNormal;
	GLuint gAlbedo;
	GLuint rboDepth;
	GLuint noiseTexture;
	GLuint ssaoFBO;
	GLuint ssaoBlurFBO;
	GLuint ssaoColorBuffer;
	GLuint ssaoColorBufferBlur;


	// object uniform location
	GLuint colorLoc;

	// light uniform location
	GLint lightLoc0;

	// flag uniform location
	GLint wireframe;

	// shader program
	GLuint program[10];

	// mouse controll
	int oldPosX;
	int oldPosY;
	int selectedPosX;
	int selectedPoxY;

	// vbo array
	vboa_t* vboa;
};
