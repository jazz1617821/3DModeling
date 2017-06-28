#include "Object.h"
#include "mymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


static vbo_t* asciiSTL(const char* filename);
static vbo_t* binarySTL(const char* filename);

// Marco
#define FILE_ERROR_MSG(){printf_s("File Error\n");}

//
//		Bounding Box
//

bndvbo_t* newBBox(void)
{
	int i;
	bndvbo_t *pBbox;

	pBbox = (bndvbo_t*)calloc(1, sizeof(bndvbo_t));
	for (i = 0; i < 3; ++i)
	{
		pBbox->min[i] = INT_MAX;
		pBbox->max[i] = INT_MIN;
	}
	for (i = 0; i < NUM_ATTRIBS; ++i)
	{
		pBbox->attribs[i] = NULL;
	}
	pBbox->dataSize[VERTEX] = 3;
	pBbox->dataSize[NORMAL] = 3;
	pBbox->dataSize[TEXCOORD] = 2;
	pBbox->dataSize[COLOR] = 4;
	pBbox->dataSize[FACE] = 4;
	pBbox->dataSize[WIREFRAME] = 3;

	return pBbox;
}

void deleteBBox(bndvbo_t** pBbox)
{
	int i;

	for (i = 0; i < NUM_ATTRIBS; ++i)
	{
		if ((*pBbox)->attribs[i] != NULL) {
			free((*pBbox)->attribs[i]);
			(*pBbox)->attribs[i] = NULL;
		}
	}
	free(*pBbox);
	*pBbox = NULL;
}

void bboxCenter(bndvbo_t bbox, float center[3])
{
	center[0] = (bbox.max[0] + bbox.min[0]) / 2.0;
	center[1] = (bbox.max[1] + bbox.min[1]) / 2.0;
	center[2] = (bbox.max[2] + bbox.min[2]) / 2.0;
}

void genBBoxVBO(bndvbo_t* pBbox)
{
	int i, j, k, tmp, color[4];
	float lx, ly, lz, transMat[16], scaleMat[16], mat[16];
	vbo_t *pBox;

	lx = pBbox->max[0] - pBbox->min[0];
	ly = pBbox->max[1] - pBbox->min[1];
	lz = pBbox->max[2] - pBbox->min[2];

	pBbox->attribs[VERTEX] = (float*)calloc(BBOX_SIZE * pBbox->dataSize[VERTEX], sizeof(float));
	pBbox->attribs[NORMAL] = (float*)calloc(BBOX_SIZE * pBbox->dataSize[NORMAL], sizeof(float));
	pBbox->attribs[WIREFRAME] = (float*)calloc(BBOX_SIZE * pBbox->dataSize[WIREFRAME], sizeof(float));

	// create bounding box polygon vbo
	pBox = newBox(NULL);
	pBbox->numVertices = pBox->numVertices;
	translate(pBbox->min[0], pBbox->min[1], pBbox->min[2], transMat);
	scale(lx, ly, lz, scaleMat);
	multMat4fv(transMat, scaleMat, mat);
	memcpy_s(pBbox->attribs[VERTEX],
			 pBbox->numVertices *
		 	 pBbox->dataSize[VERTEX] *
			 sizeof(float),
			 pBox->attribs[VERTEX],
			 pBbox->numVertices *
			 pBbox->dataSize[VERTEX] *
			 sizeof(float));
	memcpy_s(pBbox->attribs[NORMAL],
			 pBbox->numVertices *
			 pBbox->dataSize[NORMAL] *
			 sizeof(float),
			 pBox->attribs[NORMAL],
			 pBbox->numVertices *
			 pBbox->dataSize[NORMAL] *
			 sizeof(float));
	for (i = 0; i < BBOX_SIZE; ++i)
	{
		multMat4x3fv(mat, pBbox->attribs[VERTEX] + i * 3, pBbox->attribs[VERTEX] + i * 3);
	}
	deleteObject(&pBox);

	// create bounding box wireframe vbo
	pBox = newBoxWF(NULL);
	pBbox->numWFVertices = pBox->numVertices;
	pBbox->numWFLoop = 4;
	pBbox->attribs[WIREFRAME] = (float*)calloc(pBbox->numWFVertices * 
											   pBbox->dataSize[WIREFRAME],
											   sizeof(float));
	memcpy_s(pBbox->attribs[WIREFRAME],
			 pBbox->numWFVertices *
			 pBbox->dataSize[WIREFRAME] *
			 sizeof(float),
			 pBox->attribs[VERTEX],
			 pBbox->numWFVertices *
			 pBbox->dataSize[WIREFRAME] *
			 sizeof(float));

	for (i = 0; i < pBbox->numWFVertices; ++i)
	{
		multMat4x3fv(mat, pBbox->attribs[WIREFRAME] + i * 3, pBbox->attribs[WIREFRAME] + i * 3);
	}
	deleteObject(&pBox);

	pBbox->enableBuffers[VERTEX] = pBbox->enableBuffers[NORMAL] = pBbox->enableBuffers[WIREFRAME] = true;
}

//
//		Object
//

vbo_t* newObject(vbo_t *parent)
{
	int i;
	vbo_t* pObj;

	pObj = (vbo_t*)calloc(1, sizeof(vbo_t));
	pObj->bbox = newBBox();
	pObj->material = newMaterial();
	identifyMat4fv(pObj->modelMat);
	pObj->dataSize[VERTEX] = 3;
	pObj->dataSize[NORMAL] = 3;
	pObj->dataSize[TEXCOORD] = 2;
	pObj->dataSize[COLOR] = 4;
	pObj->dataSize[FACE] = 4;
	pObj->dataSize[WIREFRAME] = 3;
	for (i = 0; i < NUM_ATTRIBS; ++i)
	{
		pObj->attribs[i] = NULL;
	}

	return pObj;
}

void deleteObject(vbo_t** pObj)
{
	int i = 0;

	if (*pObj != NULL) {
		for (i = 0; i < NUM_ATTRIBS; ++i)
		{
			if ((*pObj)->attribs[i] != NULL) {
				free((*pObj)->attribs[i]);
				(*pObj)->attribs[i] = NULL;
			}
		}
		deleteBBox(&((*pObj)->bbox));
		free(*pObj);
	}
	*pObj = NULL;
}

void getTriangle(vbo_t obj, int index, float v[9])
{
	int i;

	if (index * 9 <= obj.numVertices * 3) {
		for (i = 0; i < 9; ++i)
		{
			v[i] = obj.attribs[VERTEX][index * 9 + i];
		}
	}
	else { 
		for (i = 0; i < 9; ++i) 
		{
			v[i] = 0;
		}
	}
}

void compNormals(vbo_t* pObj)
{
	// to be continued...
}

void translateVBO(vbo_t* pObj, bool isRotated)
{
	int i;
	float normalMat[16], min[3] = { INT_MAX }, max[3] = { INT_MIN };

	for (i = 0; i < pObj->numVertices; ++i)
	{
		multMat4x3fv(pObj->modelMat, pObj->attribs[VERTEX] + i * 3, pObj->attribs[VERTEX] + i * 3);
		if (isRotated) {
			compNormals(pObj);
		}
	}
	multMat4x3fv(pObj->modelMat, pObj->bbox->min, pObj->bbox->min);
	multMat4x3fv(pObj->modelMat, pObj->bbox->max, pObj->bbox->max);
	identifyMat4fv(pObj->modelMat);
}

void writeObject(const char* filename, const vbo_t pObj)
{
	FILE *fp;
	int i, ret;

	fp = fopen(filename, "wb");
	if (fp == NULL)  {
		printf_s("File not found...\n");
		return;
	}
	else {
		printf_s("Successfully load!!\n");
	}

	// write object to a binary file
	// formate:
	//     - [1 integer] number of vertices
	//	   - [NUM_ATTRIBS unsigned char] attributes data size
	//     - [NUM_ATTRIBS boolean] flag of vertex attributes
	//     - [number of true flag] * [number of vertices * 3 float] vbo data
	//     - [6 floats] bounding box
	ret = fwrite(&pObj.numVertices, sizeof(int), 1, fp);
	ret = fwrite(pObj.dataSize, sizeof(unsigned char), 4, fp);
	ret = fwrite(pObj.enableBuffers, sizeof(bool), NUM_ATTRIBS, fp);
	for (i = 0; i < NUM_ATTRIBS; ++i) {
		if (pObj.enableBuffers[i]) {
			ret = fwrite(pObj.attribs[i],
						 sizeof(float), 
						 pObj.numVertices * pObj.dataSize[i],
						 fp);
		}
	}
	ret = fwrite(pObj.bbox->min, sizeof(float), 3, fp);
	ret = fwrite(pObj.bbox->max, sizeof(float), 3, fp);
	fclose(fp);
}

void toColorID(unsigned int index, float color[4])
{
	int i;

	for (i = 0; i < 4; ++i)
	{
		color[i] = index % 256 / 255.0;
		index /= 256;
	}
}

void toObjectID(const unsigned char color[4], unsigned int* index)
{
	*index = color[0] + color[1] * 256 + color[2] * 256 * 256 + color[3] * 256 * 256 * 256;
}

void registerID(vbo_t* pObj, int* id)
{
	int i, j, k;
	float face[4];

	// create unique color for off-scene rendering picking
	// six faces
	// 6 vertices in each face
	pObj->id = *id / 6;
	pObj->bbox->attribs[COLOR] = (float*)calloc(BBOX_SIZE * pObj->bbox->dataSize[COLOR], sizeof(float));
	pObj->bbox->attribs[FACE] = (float*)calloc(BBOX_SIZE * pObj->bbox->dataSize[FACE], sizeof(float));
	for (i = 0; i < 6; ++i)
	{
		toColorID((*id)++, face);
		for (j = 0; j < 6; ++j)
		{
			for (k = 0; k < pObj->bbox->dataSize[FACE]; ++k)
			{
				pObj->bbox->attribs[FACE][i * 6 * pObj->bbox->dataSize[FACE] + j * pObj->bbox->dataSize[FACE] + k] = face[k];
			}
		}
	}
	pObj->bbox->enableBuffers[COLOR] = pObj->bbox->enableBuffers[FACE] = true;
}

void setColorVBO(float r, float g, float b, float a, vbo_t* vbo)
{
	int i;

	vbo->attribs[COLOR] = (float*)calloc(vbo->numVertices * vbo->dataSize[COLOR], sizeof(float));
	for (i = 0; i < vbo->numVertices; ++i)
	{
		vbo->attribs[COLOR][i * vbo->dataSize[COLOR]] = r;
		vbo->attribs[COLOR][i * vbo->dataSize[COLOR] + 1] = g;
		vbo->attribs[COLOR][i * vbo->dataSize[COLOR] + 2] = b;
		vbo->attribs[COLOR][i * vbo->dataSize[COLOR] + 3] = a;
	}
	vbo->enableBuffers[COLOR] = true;;
	vbo->material->isEnable = false;
}


vboa_t* newVertexBufferObjectArray()
{
	vboa_t* vboa;

	vboa = (vboa_t*)calloc(1, sizeof(vboa_t));
	vboa->capVBOs = 1;
	vboa->vbos = (vbo_t**)calloc(1, sizeof(vbo_t*));

	return vboa;
}

void deleteVertexBufferObjectArray(vboa_t** vboa)
{
	int i;

	for (i = 0; i < (*vboa)->numVBOs; ++i)
	{
		deleteObject(&(*vboa)->vbos[i]);
	}
	free((*vboa)->vbos);
	free(*vboa);
}

void addVertexBufferObject(vbo_t* vbo, vboa_t* vboa)
{
	// realloc
	if (vboa->numVBOs >= vboa->capVBOs) {
		vboa->capVBOs *= 2;
		vboa->vbos = (vbo_t**)realloc(vboa->vbos, vboa->capVBOs * sizeof(vbo_t*));
	}

	// add vbo
	vboa->vbos[vboa->numVBOs++] = vbo;
}


//
//		Drawing Stuff
//

//
//   5¢z¡X¢{2 4
//    ¢x¢¬¢x
// 0 3¢|¢w¢}1
//
static void createTile(float x, float y,  float gap, float v[18], float n[18])
{
	v[0] = x; v[1] = y; v[2] = 0;
	v[3] = x + gap; v[4] = y; v[5] = 0;
	v[6] = x; v[7] = y + gap; v[8] = 0;
	v[9] = x + gap; v[10] = y + gap; v[11] = 0;
	v[12] = x; v[13] = y + gap; v[14] = 0;
	v[15] = x + gap; v[16] = y; v[17] = 0;

	n[0]  = 0;  n[1]  = 0;   n[2]  = 1;
	n[3]  = 0;  n[4]  = 0;   n[5]  = 1;
	n[6]  = 0;  n[7]  = 0;   n[8]  = 1;
	n[9]  = 0;  n[10] = 0;   n[11] = 1;
	n[12] = 0;  n[13] = 0;   n[14] = 1;
	n[15] = 0;  n[16] = 0;   n[17] = 1;
}

//
//  v3¢z¡X¢{v2
//    ¢x¢¬¢x
//  v0¢|¢w¢}v1
//
//
//   5¢z¡X¢{2 4
//    ¢x¢¬¢x
// 0 3¢|¢w¢}1
//
void createPolygonVBO(float v0[3], float v1[3], float v2[3], float v3[3], float v[18], float vn[18])
{
	int i;
	float e0[3], e1[3], normal[3];

	memcpy(v, v0, 3 * sizeof(float));
	memcpy(v + 3, v1, 3 * sizeof(float));
	memcpy(v + 6, v2, 3 * sizeof(float));
	memcpy(v + 9, v0, 3 * sizeof(float));
	memcpy(v + 12, v2, 3 * sizeof(float));
	memcpy(v + 15, v3, 3 * sizeof(float));

	e0[0] = v1[0] - v0[0];
	e0[1] = v1[1] - v0[1];
	e0[2] = v1[2] - v0[2];
	e1[0] = v2[0] - v0[0];
	e1[1] = v2[1] - v0[1];
	e1[2] = v2[2] - v0[2];
	cross3fv(e1, e0, normal);
	for (i = 0; i < 18; i += 3)
	{
		memcpy(vn + i, normal, 3 * sizeof(float));
	}
}

vbo_t* newPlane(int xSize, int ySize, float gap)
{
	int i, j, index;
	vbo_t *pObj;
	float tileV[18], tileN[18], mat[16], tmp;

	pObj = newObject(NULL);
	pObj->numVertices = xSize * ySize * 6;
	pObj->attribs[VERTEX] = (float*)calloc(pObj->numVertices * 3, sizeof(float));
	pObj->attribs[NORMAL] = (float*)calloc(pObj->numVertices * 3, sizeof(float));
	pObj->enableBuffers[VERTEX] = pObj->enableBuffers[NORMAL] = true;
	pObj->enable = true;
	strcpy(pObj->name, "plane");
	pObj->bbox->max[0] = xSize * gap;
	pObj->bbox->max[1] = ySize * gap;
	pObj->bbox->max[2] = 0;

	index = 0;
	for (i = 0; i < xSize; ++i)
	{
		for (j = 0; j < ySize; ++j)
		{
			createTile(i * gap, j * gap, gap, tileV, tileN);
			memcpy(pObj->attribs[VERTEX] + index, tileV, 18 * sizeof(float));
			memcpy(pObj->attribs[NORMAL] + index, tileN, 18 * sizeof(float));
			index += 18;
		}
	}

	return pObj;
}

vbo_t* newPlainWF(int xSize, int ySize, float gap)
{
	int i, j, k, index;
	vbo_t *pObj;
	float tileV[18], tileN[18], mat[16], tmp;

	pObj = newObject(NULL);
	pObj->numVertices = 4 * xSize * ySize + 2 * (xSize + ySize);
	pObj->attribs[VERTEX] = (float*)calloc(pObj->numVertices * 3, sizeof(float));
	pObj->attribs[NORMAL] = (float*)calloc(pObj->numVertices * 3, sizeof(float));
	pObj->enableBuffers[VERTEX] = pObj->enableBuffers[NORMAL] = true;
	pObj->enable = true;
	strcpy(pObj->name, "plane");
	pObj->bbox->max[0] = xSize * gap;
	pObj->bbox->max[1] = ySize * gap;
	pObj->bbox->max[2] = 0;

	// x-direct lines
	for (i = 0; i <= ySize; ++i)
	{
		for (j = 0; j < xSize; ++j)
		{
			// v1
			pObj->attribs[VERTEX][(i * xSize * 2 + j) * 3] = j * gap;
			pObj->attribs[VERTEX][(i * xSize * 2 + j) * 3 + 1] = i * gap;
			pObj->attribs[VERTEX][(i * xSize * 2 + j) * 3 + 2] = 0;
			// v2
			pObj->attribs[VERTEX][(i * xSize * 2 + j + 1) * 3] = (j + 1) * gap;
			pObj->attribs[VERTEX][(i * xSize * 2 + j + 1) * 3 + 1] = i * gap;
			pObj->attribs[VERTEX][(i * xSize * 2 + j + 1) * 3 + 2] = 0;
		}
	}

	// y-direct lines
	for (i = 0; i <= xSize; ++i)
	{
		for (j = 0; j < ySize; ++j)
		{
			// v1
			pObj->attribs[VERTEX][(i * ySize * 2 + j) * 3] = i * gap;
			pObj->attribs[VERTEX][(i * ySize * 2 + j) * 3 + 1] = j * gap;
			pObj->attribs[VERTEX][(i * ySize * 2 + j) * 3 + 2] = 0;
			// v2
			pObj->attribs[VERTEX][(i * ySize * 2 + j + 1) * 3] = i * gap;
			pObj->attribs[VERTEX][(i * ySize * 2 + j + 1) * 3 + 1] = (j + 1) * gap;
			pObj->attribs[VERTEX][(i * ySize * 2 + j + 1) * 3 + 2] = 0;
		}
	}

	return pObj;
}

vbo_t* newBox(bool face[8])
{
	vbo_t* pObj;
	int i, j, k, numFaces, numVertices;
	//	x, y, z position of eight vertices
	float v[24] = { 0, 0, 1,	//	0
					0, 1, 1,	//	1
					1, 1, 1,	//	2
					1, 0, 1,	//	3
					0, 0, 0,	//	4
					0, 1, 0,	//	5
					1, 1, 0,	//	6
					1, 0, 0 };	//	7

	//	six box face normals
	float n[24] = {  1,  0,  0,		//	0
					-1,  0,  0,		//	1
					 0,  1,  0,		//	2
					 0, -1,  0,		//	3
					 0,  0,  1,		//	4
					 0,  0, -1,     //	5
					 0,  0,  0,		// better normal
					 0,  0,  0, };  // better normal

	if (face == NULL){
		numFaces = 6;
	}
	else {
		numFaces = 0;
		for (i = 0; i < 6; ++i) {
			face[i] ? ++numFaces : 0;
		}
	}

	int vidx[36] = { 2, 3, 7,		//	frnot
					 7, 6, 2,
					 5, 4, 0,		//	back
					 0, 1, 5,
					 6, 5, 1,		//	top
					 1, 2, 6,
					 3, 0, 4,		//	bottom
					 4, 7, 3,
					 1, 0, 3,		//	right
					 3, 2, 1,
					 4, 5, 6,		//	left
					 6, 7, 4 };

	int nidx[36] = { 0, 0, 0,		//	fonrt
					 0, 0, 0,
					 1, 1, 1,		//	back
					 1, 1, 1,
					 2, 2, 2,		//	top
					 2, 2, 2,
					 3, 3, 3,		//	bottom
					 3, 3, 3,
					 4, 4, 4,		//	right
					 4, 4, 4,
					 5, 5, 5,		//	left
					 5, 5, 5 };

	pObj = newObject(NULL);
	strcpy_s(pObj->name, "box");
	pObj->attribs[VERTEX] = (float*)calloc(numFaces * 18, sizeof(float));
	pObj->attribs[NORMAL] = (float*)calloc(numFaces * 18, sizeof(float));
	numVertices = 0;
	for (i = 0; i < 6; ++i) {
		if (face == NULL || face[i]) {
			for (j = 0; j < 6; ++j) {
				for (k = 0; k < 3; ++k) 
				{
					pObj->attribs[VERTEX][numVertices * 3 + k] = v[vidx[i * 6 + j] * 3 + k];
					pObj->attribs[NORMAL][numVertices * 3 + k] = n[nidx[i * 6 + j] * 3 + k];
				}
				++numVertices;
			}
		}
	}

	pObj->numVertices = numVertices;
	pObj->bbox->min[0] = pObj->bbox->min[1] = pObj->bbox->max[2] = 1;
	pObj->bbox->max[0] = pObj->bbox->max[1] = pObj->bbox->max[2] = 1;
	pObj->enableBuffers[VERTEX] = pObj->enableBuffers[NORMAL] = true;
	return pObj;
}

vbo_t* newBoxWF(bool face[8])
{
	vbo_t* pObj;
	int i, j, k, numFaces, numVertices;
	//	x, y, z position of eight vertices
	float v[24] = { 0, 0, 1,	//	0
					0, 1, 1,	//	1
					1, 1, 1,	//	2
					1, 0, 1,	//	3
					0, 0, 0,	//	4
					0, 1, 0,	//	5
					1, 1, 0,	//	6
					1, 0, 0 };	//	7

	if (face == NULL) {
		numFaces = 6;
	}
	else {
		numFaces = 0;
		for (i = 0; i < 6; ++i) {
			face[i] ? ++numFaces : 0;
		}
	}

	int vidx[24] = { 2, 3, 7, 6,		//	frnot
					 5, 4, 0, 1,		//	back
					 6, 5, 1, 2,		//	top
					 3, 0, 4, 7,		//	bottom
					 1, 0, 3, 2,		//	right
					 4, 5, 6, 7 };		//	left

	pObj = newObject(NULL);
	pObj->attribs[VERTEX] = (float*)calloc(numFaces * 12, sizeof(float));
	numVertices = 0;
	for (i = 0; i < 6; ++i) {
		if (face == NULL || face[i]) {
			for (j = 0; j < 4; ++j) {
				for (k = 0; k < 3; ++k)
				{
					pObj->attribs[VERTEX][numVertices * 3 + k] = v[vidx[i * 4 + j] * 3 + k];
				}
				++numVertices;
			}
		}
	}
	pObj->numVertices = numVertices;
	pObj->bbox->min[0] = pObj->bbox->min[1] = pObj->bbox->min[2] = 1;
	pObj->bbox->max[0] = pObj->bbox->max[1] = pObj->bbox->max[2] = 1;
	pObj->enableBuffers[VERTEX] = true;
	return pObj;
}

void initBox(vbo_t *pObj, bool face[8])
{
	if (pObj == NULL) return;
	int i, j, k, numFaces, numVertices;
	//	x, y, z position of eight vertices
	float v[24] = { 0, 0, 1,	//	0
		0, 1, 1,	//	1
		1, 1, 1,	//	2
		1, 0, 1,	//	3
		0, 0, 0,	//	4
		0, 1, 0,	//	5
		1, 1, 0,	//	6
		1, 0, 0 };	//	7

	//	six box face normals
	float n[24] = { 1, 0, 0,		//	0
		-1, 0, 0,		//	1
		0, 1, 0,		//	2
		0, -1, 0,		//	3
		0, 0, 1,		//	4
		0, 0, -1,     //	5
		0, 0, 0,		// better normal
		0, 0, 0, };  // better normal

	if (face == NULL){
		numFaces = 6;
	}
	else {
		numFaces = 0;
		for (i = 0; i < 6; ++i) {
			face[i] ? ++numFaces : 0;
		}
	}

	int vidx[36] = { 2, 3, 7,		//	frnot
		7, 6, 2,
		5, 4, 0,		//	back
		0, 1, 5,
		6, 5, 1,		//	top
		1, 2, 6,
		3, 0, 4,		//	bottom
		4, 7, 3,
		1, 0, 3,		//	right
		3, 2, 1,
		4, 5, 6,		//	left
		6, 7, 4 };

	int nidx[36] = { 0, 0, 0,		//	fonrt
		0, 0, 0,
		1, 1, 1,		//	back
		1, 1, 1,
		2, 2, 2,		//	top
		2, 2, 2,
		3, 3, 3,		//	bottom
		3, 3, 3,
		4, 4, 4,		//	right
		4, 4, 4,
		5, 5, 5,		//	left
		5, 5, 5 };

	memset(pObj->attribs[VERTEX], 0, 108 * sizeof(float));
	memset(pObj->attribs[NORMAL], 0, 108 * sizeof(float));
	numVertices = 0;
	for (i = 0; i < 6; ++i) {
		if (face == NULL || face[i]) {
			for (j = 0; j < 6; ++j) {
				for (k = 0; k < 3; ++k)
				{
					pObj->attribs[VERTEX][numVertices * 3 + k] = v[vidx[i * 6 + j] * 3 + k];
					pObj->attribs[NORMAL][numVertices * 3 + k] = n[nidx[i * 6 + j] * 3 + k];
				}
				++numVertices;
			}
		}
	}

	pObj->numVertices = numVertices;
	pObj->bbox->min[0] = pObj->bbox->min[1] = pObj->bbox->min[2] = 1;
	pObj->bbox->max[0] = pObj->bbox->max[1] = pObj->bbox->max[2] = 1;
	pObj->enableBuffers[VERTEX] = pObj->enableBuffers[NORMAL] = true;
}

void initBoxWF(vbo_t *pObj, bool face[8])
{
	if (pObj == NULL) return;
	int i, j, k, numFaces, numVertices;
	//	x, y, z position of eight vertices
	float v[24] = { 0, 0, 1,	//	0
		0, 1, 1,	//	1
		1, 1, 1,	//	2
		1, 0, 1,	//	3
		0, 0, 0,	//	4
		0, 1, 0,	//	5
		1, 1, 0,	//	6
		1, 0, 0 };	//	7

	if (face == NULL) {
		numFaces = 6;
	}
	else {
		numFaces = 0;
		for (i = 0; i < 6; ++i) {
			face[i] ? ++numFaces : 0;
		}
	}

	int vidx[24] = { 2, 3, 7, 6,		//	frnot
		5, 4, 0, 1,		//	back
		6, 5, 1, 2,		//	top
		3, 0, 4, 7,		//	bottom
		1, 0, 3, 2,		//	right
		4, 5, 6, 7 };		//	left

	numVertices = 0;
	for (i = 0; i < 6; ++i) {
		if (face == NULL || face[i]) {
			for (j = 0; j < 4; ++j) {
				for (k = 0; k < 3; ++k)
				{
					pObj->attribs[VERTEX][numVertices * 3 + k] = v[vidx[i * 4 + j] * 3 + k];
				}
				++numVertices;
			}
		}
	}
	pObj->numVertices = numVertices;
	pObj->bbox->min[0] = pObj->bbox->min[1] = pObj->bbox->min[2] = 1;
	pObj->bbox->max[0] = pObj->bbox->max[1] = pObj->bbox->max[2] = 1;
	pObj->enableBuffers[VERTEX] = true;
}