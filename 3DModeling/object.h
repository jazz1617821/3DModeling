#ifndef _OBJECT_H
#define _OBJECT_H

#include <GL\glew.h>
#include "material.h"

#define OBJECT_NAME_SIZE 50
#define BBOX_SIZE 36

enum VERTEX_ATTRIBS {VERTEX, NORMAL, TEXCOORD, COLOR, FACE, WIREFRAME, NUM_ATTRIBS};

typedef struct AxisAlignedBoundingBoxVBO
{
	float min[3];
	float max[3];
	float *attribs[NUM_ATTRIBS];
	bool enable;
	bool enableBuffers[NUM_ATTRIBS];
	unsigned char dataSize[NUM_ATTRIBS];
	GLuint buffers[NUM_ATTRIBS];
	int numVertices;
	int numWFVertices;
	int numWFLoop;
} bndvbo_t;

typedef struct VBO 
{
	char name[OBJECT_NAME_SIZE];
	bool polygonDetect;
	bool bboxDetect;
	bool enableBuffers[NUM_ATTRIBS];
	bool enable;
	bool wfEnable;
	bool isGenBuffer;
	int id;
	int numVertices;
	int numWFVertices;
	int numWFLoop;
	unsigned char dataSize[NUM_ATTRIBS];
	GLuint buffers[NUM_ATTRIBS];
	float worldCoord[3];
	float eulerAngle[3];
	float quaternion[4];
	float *attribs[NUM_ATTRIBS];
	float modelMat[16];
	mtl_t *material;
	bndvbo_t *bbox;
	float clip[4];
	bool isClip;
} vbo_t;

typedef struct VBOArray
{
	int numVBOs;
	int capVBOs;
	vbo_t** vbos;
}vboa_t;


//
//		Bounding Box
//

bndvbo_t* newBBox(void);

void deleteBBox(bndvbo_t **pBbox);

void bboxCenter(bndvbo_t bbox, float center[3]);

void genBBoxVBO(bndvbo_t *pBbox);

//
//		Object
//

vbo_t* newObject(vbo_t *parent);

void deleteObject(vbo_t **pObj);

void getTriangle(vbo_t obj, int index, float v[9]);

void compNormals(vbo_t *pObj);

void translateVBO(vbo_t *pObj, bool isRotated);

void writeObject(const char *filename, const vbo_t pObj);

void toColorID(unsigned int index, float color[4]);

void toObjectID(const unsigned char color[4], unsigned int *index);

void registerID(vbo_t *pObj, int *id);

void setColorVBO(float r, float g, float b, float a, vbo_t* vbo);


vboa_t* newVertexBufferObjectArray();
void deleteVertexBufferObjectArray(vboa_t** vboa);
void addVertexBufferObject(vbo_t* vbo, vboa_t* vboa);

//vbo_t* loadVoxelModel(const char* filenmae);


//
//		Drawing Stuff
//

void createPolygonVBO(float v0[3], float v1[3], float v2[3], float v3[3], float v[18], float vn[18]);

vbo_t* newPlane(int xSize, int ySize, float gap);

vbo_t* newBox(bool face[8]);

vbo_t* newBoxWF(bool face[8]);

void initBox(vbo_t *pObj, bool face[8]);

void initBoxWF(vbo_t* pObj, bool face[8]);

#endif