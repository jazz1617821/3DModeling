#include "meshmodel.h"
#include "mymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

/////////////////////////////
//     gloabal variable    //
/////////////////////////////

static int g_modelBvhID = 0;
static int g_modelID = 0;

/////////////////////////////
//        functions        //
/////////////////////////////

//static int addTexcoordArray(float vt[3], float precision); // no implement

static modelbvh_t* deleteBVH(modelbvh_t* root);

static modelobj_t* asciiSTL(const char* filename);
static modelobj_t* binarySTL(const char* filename);


void initAABB(aabb_t* bbox)
{
	bbox->max[0] = bbox->max[1] = bbox->max[2] = INT_MIN;
	bbox->min[0] = bbox->min[1] = bbox->min[2] = INT_MAX;
}

bool addBound(aabb_t* bbox, float val[3])
{
	bool isChanged = false;
	int i;

	for (i = 0; i < 3; ++i)
	{
		if (bbox->max[i] <= val[i]) {
			bbox->max[i] = val[i];
			isChanged = true;
		}
		if (bbox->min[i] >= val[i]) {
			bbox->min[i] = val[i];
			isChanged = true;
		}
	}

	return isChanged;
}

void setAABB(aabb_t* dest, aabb_t* src)
{
	int i;

	for (i = 0; i < 3; ++i)
	{
		dest->max[i] = src->max[i];
		dest->min[i] = src->min[i];
	}
}

void aabbCenter(aabb_t bbox, float center[3])
{
	center[0] = (bbox.max[0] + bbox.min[0]) / 2.0;
	center[1] = (bbox.max[1] + bbox.min[1]) / 2.0;
	center[2] = (bbox.max[2] + bbox.min[2]) / 2.0;
}

void initVertexArray(va_t* va)
{
	if (va == NULL) return;
	va->numVertices = 0;
	if (va->data != NULL)
		free(va->data);
	va->data = NULL;
}


int addVertexArray3D(float v[3], va_t* va)
{
	int i;
	int index;

	// initial vertexArray
	if (va->data == NULL) {
		va->data = (float*)calloc(va->capVertices, sizeof(float));
	}
	else if (va->numVertices + 3 >= va->capVertices) {
		va->capVertices *= 2;
		va->data = (float*)realloc(va->data, va->capVertices * sizeof(float));
	}

	index = va->numVertices;
	// add new vertex x, y, z
	for (i = 0; i < 3; ++i)
	{
		va->data[va->numVertices++] = v[i];
	}
	return index;
}

model_t* newModel()
{
	model_t* model;

	model = (model_t*)calloc(1, sizeof(model_t));
	model->id = -1;
	memset(model->name, '\0', 512);
	model->vertexArray.numVertices = 0;
	model->vertexArray.capVertices = 256;
	model->vertexArray.data = NULL;	
	model->normalArray.numVertices = 0;
	model->normalArray.capVertices = 256;
	model->normalArray.data = NULL;
	model->texcoordArray.numVertices = 0;
	model->texcoordArray.capVertices = 64;
	model->texcoordArray.data = NULL;

	initAABB(&model->bbox);

	return model;
}

void deleteModel(model_t** model)
{
	initVertexArray(&(*model)->vertexArray);
	initVertexArray(&(*model)->normalArray);
	initVertexArray(&(*model)->texcoordArray);
	
	if ((*model)->faces != NULL) {
		free((*model)->faces);
	}
	if ((*model)->halfedges != NULL) {
		free((*model)->halfedges);
	}
	free(*model);
	*model = NULL;	
}

modelbvh_t* newModelBVH()
{
	modelbvh_t* bvh;

	bvh = (modelbvh_t*)calloc(1, sizeof(modelbvh_t));
	memset(bvh->name, '\0', 512);
	bvh->id = -1;
	initAABB(&bvh->bbox);

	return bvh;
}

static modelbvh_t* deleteBVH(modelbvh_t* root)
{
	int i;

	if (root->children != NULL) {
		for (i = 0; i < root->numChildren; ++i)
		{
			root->children[i] = deleteBVH(root->children[i]);
		}
	}
	// delete
	if (root->model != NULL) {
		deleteModel(&root->model);
	}
	free(root);
	return NULL;
}

void deleteModelBVH(modelbvh_t** bvh)
{
	*bvh = deleteBVH(*bvh);
}

modelobj_t* newModelObject()
{
	modelobj_t* obj;

	obj = (modelobj_t*)calloc(1, sizeof(modelobj_t));
	memset(obj->name, '\0', 512);

	return obj;
}

void deleteModelObject(modelobj_t** obj)
{
	int i;
	
	(*obj)->models = NULL;
	deleteModelBVH(&(*obj)->root);
	free(*obj);
	*obj = NULL;
}

moa_t* newModelObjectArray()
{
	moa_t* moa;

	moa = (moa_t*)calloc(1, sizeof(moa_t));
	moa->capObjects = 1;
	moa->objects = (modelobj_t**)calloc(1, sizeof(modelobj_t*));
	
	return moa;
}

void deleteModelObjectArray(moa_t** moa)
{
	int i;
	modelobj_t* obj;

	for (i = 0; i < (*moa)->numObjects; ++i)
	{
		deleteModelObject(&(*moa)->objects[i]);
	}
	free(*moa);
	*moa = NULL;
}

static modelobj_t* asciiSTL(const char* filename)
{
	FILE *fp;
	char buffer[1024];
	char* ptr;
	char* prev;
	int i, j, k, num, index;
	float val[3], offset[3], transMat[16];
	modelobj_t* obj;
	modelbvh_t* bvh;
	model_t* model;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Can not open %s...\\n", filename);
		return NULL;
	}
	
	obj = newModelObject();
	bvh = newModelBVH();
	model = newModel();

	// get filename
	strcpy(buffer, filename);
	ptr = strtok(buffer, "\\\\/");
	while (ptr != NULL) {
		prev = ptr;
		ptr = strtok(NULL, "\\\\/");
	}
	strcpy(model->name, strtok(prev, "."));

	// count how many tirangles in this file
	num = 0;
	while (fgets(buffer, 1000, fp) != NULL) {
		ptr = strstr(buffer, "facet normal");
		if (ptr != NULL) {
			++num;
		}
	}
	rewind(fp);
	model->numFaces = num;
	model->faces = (face_t*)calloc(model->numFaces, sizeof(face_t));

	// solid name
	ptr = fgets(buffer, 1000, fp);
	//ptr = strstr(buffer, "solid");
	//ret = sscanf_s(ptr, "solid %s\\n", buffer);

	// facet loop
	initAABB(&model->bbox);
	for (i = 0; i < num; ++i)
	{
		ptr = fgets(buffer, 1000, fp);
		ptr = strstr(buffer, "facet normal");
		sscanf(ptr, "facet normal %f %f %f", &val[0], &val[1], &val[2]);
		index = addVertexArray3D(val, &model->normalArray);
		for (j = 0; j < 3; ++j) 
		{
			model->faces[i].vn[j] = index;
		} // end normal
		ptr = fgets(buffer, 1000, fp);
		ptr = strstr(buffer, "outer loop");
		for (j = 0; j < 3; ++j) 
		{
			ptr = fgets(buffer, 1000, fp);
			ptr = strstr(buffer, "vertex");
			sscanf(ptr, "vertex %f %f %f", &val[0], &val[1], &val[2]);
			addBound(&model->bbox, val);
			index = addVertexArray3D(val, &model->vertexArray);
			model->faces[i].v[j] = index;
		} // end vertex
		ptr = fgets(buffer, 1000, fp);
		ptr = strstr(buffer, "endloop");
		ptr = fgets(buffer, 1000, fp);
		ptr = strstr(buffer, "endfacet");
		
	}
	fclose(fp);

	// translate model above plane
	offset[0] = -(model->bbox.max[0] + model->bbox.min[0]) / 2.0;
	offset[1] = -model->bbox.min[1];
	offset[2] = -(model->bbox.max[2] + model->bbox.min[2]) / 2.0;
	translate(offset[0], offset[1], offset[2], transMat);

	for (i = 0; i < model->vertexArray.numVertices; i += 3)
	{
		multMat4x3fv(transMat, model->vertexArray.data + i, model->vertexArray.data + i);
	}
	multMat4x3fv(transMat, model->bbox.max, model->bbox.max);
	multMat4x3fv(transMat, model->bbox.min, model->bbox.min);

	model->id = g_modelID++;
	bvh->model = model;
	strcpy(bvh->name, model->name);
	bvh->numChildren = 0;
	bvh->children = NULL;
	bvh->parent = NULL;
	setAABB(&bvh->bbox, &model->bbox);

	obj->numModels = 1;
	obj->models = (model_t**)calloc(1, sizeof(obj->numModels));
	obj->root = bvh;
	obj->models[0] = model;
	strcpy(obj->name, model->name);

	return obj;
}


static modelobj_t* binarySTL(const char* filename)
{
	FILE *fp;
	char buffer[1024];
	char* ptr;
	char* prev;
	unsigned short attr;
	int i, j, k, num, index;
	float val[3];
	modelobj_t* obj;
	modelbvh_t* bvh;
	model_t* model;

	fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Can not open %s...\\n", filename);
		return NULL;
	}

	obj = newModelObject();
	bvh = newModelBVH();
	model = newModel();

	// get filename
	strcpy(buffer, filename);
	ptr = strtok(buffer, "\\\\/");
	while (ptr != NULL) {
		prev = ptr;
		ptr = strtok(NULL, "\\\\/");
	}
	strcpy(model->name, strtok(prev, "."));

	// header [80 bytes]
	fread(buffer, sizeof(char), 80, fp);
	// numOfTri [4 bytes]
	fread(&num, sizeof(int), 1, fp);
	model->numFaces = num;
	model->faces = (face_t*)calloc(model->numFaces, sizeof(face_t));
	
	// facet loop
	initAABB(&model->bbox);
	for (i = 0; i < num; ++i)
	{
		// normal [4 bytes * 3]
		fread(val, sizeof(float), 3, fp);
		index = addVertexArray3D(val, &model->normalArray);
		for (j = 0; j < 3; ++j)
		{
			model->faces[i].vn[j] = index;
		} // end normal

		// vertex [4 bytes * 3 * 3]
		for (j = 0; j < 3; ++j)
		{
			fread(val, sizeof(float), 3, fp);
			index = addVertexArray3D(val, &model->vertexArray);
			model->faces[i].v[j] = index;
			addBound(&model->bbox, val);
		}// end vertex
		// attributes [2 bytes]
		fread(&attr, sizeof(unsigned short), 1, fp);
	}
	fclose(fp);

	model->id = g_modelID++;
	bvh->id = g_modelBvhID++;
	bvh->model = model;
	strcpy(bvh->name, model->name);
	bvh->numChildren = 0;
	bvh->parent = NULL;
	bvh->children = NULL;
	setAABB(&bvh->bbox, &model->bbox);

	obj->numModels = 1;
	obj->models = (model_t**)calloc(1, sizeof(obj->numModels));
	obj->root = bvh;
	obj->models[0] = model;
	strcpy(obj->name, model->name);

	return obj;
}

void addModelObject(modelobj_t* obj, moa_t* moa)
{
	// realloc
	if (moa->numObjects >= moa->capObjects) {
		moa->capObjects *= 2;
		moa->objects =
			(modelobj_t**)realloc(moa->objects,
				moa->capObjects * sizeof(modelobj_t*));
	}
	// add
	moa->objects[moa->numObjects] = obj;
	moa->numObjects++;
}


modelobj_t* loadSTL(const char* filename)
{
	FILE *fp = fopen(filename, "r");
	char buffer[100];

	if (fp == NULL) {
		return NULL;
	}
	fgets(buffer, 100, fp);
	fclose(fp);
	if (strstr(buffer, "solid")) {
		return asciiSTL(filename);
	}
	else {
		return binarySTL(filename);
	}
}

void getTriangle(model_t* model, int index, float triVert[9])
{
	int i, j;

	for (i = 0; i < 3; ++i)
		for (j = 0; j < 3; ++j)
			triVert[i * 3 + j] = model->vertexArray.data[model->faces[index].v[i] + j];
}

static modelbvh_t* calBVHBounding(modelbvh_t* bvh)
{
	int i, j;
	modelbvh_t* childBVH;
	aabb_t newBnd;

	newBnd.max[0] = newBnd.max[1] = newBnd.max[2] = INT_MIN;
	newBnd.min[0] = newBnd.min[1] = newBnd.min[2] = INT_MAX;
	if (bvh->numChildren > 0) {
		for (i = 0; i < bvh->numChildren; ++i)
		{
			childBVH = calBVHBounding(bvh->children[i]);
			addBound(&newBnd, childBVH->bbox.max);
			addBound(&newBnd, childBVH->bbox.min);
		}
	}
	else if(bvh->model != NULL) {
		setAABB(&bvh->bbox, &bvh->model->bbox);
	}
	return bvh;
}

void turnZUp(modelobj_t* obj)
{
	int i, j, k, l, index;
	float tmp, normal[3], vertex[3], triVertex[9], offset[3], transMat[16];
	model_t* model;
	aabb_t newBnd;

	for (i = 0; i < obj->numModels; ++i)
	{
		model = obj->models[i];
		// change y, z
		for (j = 0; j < model->numFaces; ++j)
		{
			for (k = 0; k < 3; ++k)
			{
				tmp = model->vertexArray.data[model->faces[j].v[k] + 1];

				model->vertexArray.data[model->faces[j].v[k] + 1] =
					model->vertexArray.data[model->faces[j].v[k] + 2];

				model->vertexArray.data[model->faces[j].v[k] + 2] = -tmp;
			}
		}

		initVertexArray(&model->normalArray);
		// calculate normals
		for (j = 0; j < model->numFaces; ++j)
		{
			getTriangle(model, j, triVertex);
			compNormal(triVertex, normal);
			index = addVertexArray3D(normal, &model->normalArray);
			for (k = 0; k < 3; ++k)
			{
				model->faces[j].vn[k] = index;
			} // end loop k
		} // end loop j

		initAABB(&newBnd);
		for (i = 0; i < model->vertexArray.numVertices; i += 3)
		{
			addBound(&newBnd, model->vertexArray.data + i);
		}
		setAABB(&model->bbox, &newBnd);

		// translate model above plane
		offset[0] = -(model->bbox.max[0] + model->bbox.min[0]) / 2.0;
		offset[1] = -model->bbox.min[1];
		offset[2] = -(model->bbox.max[2] + model->bbox.min[2]) / 2.0;
		translate(offset[0], offset[1], offset[2], transMat);
		for (i = 0; i < model->vertexArray.numVertices; i += 3)
		{
			multMat4x3fv(transMat, model->vertexArray.data + i, model->vertexArray.data + i);
		}
		multMat4x3fv(transMat, model->bbox.max, model->bbox.max);
		multMat4x3fv(transMat, model->bbox.min, model->bbox.min);
	} // end loop i
	calBVHBounding(obj->root);
}

void getVertex3D(va_t va, int index, float v[3])
{
	v[0] = va.data[index];
	v[1] = va.data[index + 1];
	v[2] = va.data[index + 2];
}

void calTriangleNormal(float v0[3], float v1[3], float v2[3], float vn[3])
{
	float a[3], b[3], normal[3];

	a[0] = v1[0] - v0[0];
	a[1] = v1[1] - v0[1];
	a[2] = v1[2] - v0[2];
	b[0] = v2[0] - v0[0];
	b[1] = v2[1] - v0[1];
	b[2] = v2[2] - v0[2];

	cross3fv(a, b, normal);

	vn[0] = normal[0];
	vn[1] = normal[1];
	vn[2] = normal[2];
}