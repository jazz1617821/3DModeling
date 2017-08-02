#include "voxelvbo.h"
#include "mymath.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define X 0
#define Y 1
#define Z 2


vbo_t* createVoxelVBO(vobj_t* vo)
{
	bool neighbor[26];
	int i, j, k, l, m, values[3], index, count, numFaces, resolution[3];
	float translateMat[16], scaleMat[16], ids[144], face[4], origin[3];
	vdata_t* vd = vo->vd;
	voxel_t* vox = vo->vd->rawdata;
	vbo_t* vbo;
	vbo_t* boxVBO;
	vbo_t* boxWFVBO;

	origin[X] = vo->min_bound[X];
	origin[Y] = vo->min_bound[Y];
	origin[Z] = vo->min_bound[Z];


	// count drawing faces
	numFaces = 0;
	for (i = 0; i < vd->resolution[Z]; ++i)
	{
		for (j = 0; j < vd->resolution[Y]; ++j)
		{
			for (k = 0; k < vd->resolution[X]; ++k)
			{
				index = k + j * vd->resolution[X] + i * vd->resolution[X] * vd->resolution[Y];
				if (vd->rawdata[index].data == VOX_EMPTY) continue;
				(k + 1 < vd->resolution[X] && vd->rawdata[index + 1].data) ?
					0 : numFaces++;
				(k > 1 && vd->rawdata[index - 1].data) ?
					0 : numFaces++;
				(j + 1 < vd->resolution[Y] && vd->rawdata[index + vd->resolution[X]].data) ?
					0 : numFaces++;
				(j > 1 && vd->rawdata[index - vd->resolution[X]].data) ?
					0 : numFaces++;
				(i + 1 < vd->resolution[Z] && vd->rawdata[index + vd->resolution[X] * vd->resolution[Y]].data) ?
					0 : numFaces++;
				(i > 1 && vd->rawdata[index - vd->resolution[X] * vd->resolution[Y]].data) ?
					0 : numFaces++;
			}
		}
	}


	// creat vbo
	vbo = newObject(NULL);
	strcpy_s(vbo->name, vd->name);
	vbo->attribs[VERTEX] = (float*)calloc(numFaces * 108, sizeof(float));
	vbo->attribs[NORMAL] = (float*)calloc(numFaces * 108, sizeof(float));
	vbo->attribs[WIREFRAME] = (float*)calloc(numFaces * 108, sizeof(float));
	vbo->attribs[FACE] = (float*)calloc(numFaces * 144, sizeof(float));
	boxVBO = newBox(NULL);
	boxWFVBO = newBoxWF(NULL);
	scale(vd->voxelsize[X], vd->voxelsize[Y], vd->voxelsize[Z], scaleMat);
	vbo->numVertices = 0;
	vbo->numWFLoop = 4;

	count = 0;
	for (i = 0; i < vd->resolution[Z]; ++i)
	{
		for (j = 0; j < vd->resolution[Y]; ++j)
		{
			for (k = 0; k < vd->resolution[X]; ++k)
			{
				index = k + j * vd->resolution[X] + i * vd->resolution[X] * vd->resolution[Y];
				if (vd->rawdata[index].data == VOX_EMPTY) continue;
				neighbor[0] = (k + 1 < vd->resolution[X] && vd->rawdata[index + 1].data) ?
					false : true;
				neighbor[1] = (k > 1 && vd->rawdata[index - 1].data) ?
					false : true;
				neighbor[2] = (j + 1 < vd->resolution[Y] && vd->rawdata[index + vd->resolution[X]].data) ?
					false : true;
				neighbor[3] = (j > 1 && vd->rawdata[index - vd->resolution[X]].data) ?
					false : true;
				neighbor[4] = (i + 1 < vd->resolution[Z] && vd->rawdata[index + vd->resolution[X] * vd->resolution[Y]].data) ?
					false : true;
				neighbor[5] = (i > 1 && vd->rawdata[index - vd->resolution[X] * vd->resolution[Y]].data) ?
					false : true;
				initBox(boxVBO, neighbor);
				initBoxWF(boxWFVBO, neighbor);
				translate(origin[0] + k * vd->voxelsize[X],
					origin[1] + j * vd->voxelsize[Y],
					origin[2] + i * vd->voxelsize[Z],
					translateMat);
				multMat4fv(translateMat, scaleMat, boxVBO->modelMat);
				multMat4fv(translateMat, scaleMat, boxWFVBO->modelMat);
				translateVBO(boxVBO, false);
				translateVBO(boxWFVBO, false);
				memcpy_s(vbo->attribs[VERTEX] + vbo->numVertices * 3, boxVBO->numVertices * 3 * sizeof(float), boxVBO->attribs[VERTEX], boxVBO->numVertices * 3 * sizeof(float));
				memcpy_s(vbo->attribs[NORMAL] + vbo->numVertices * 3, boxVBO->numVertices * 3 * sizeof(float), boxVBO->attribs[NORMAL], boxVBO->numVertices * 3 * sizeof(float));
				memcpy_s(vbo->attribs[WIREFRAME] + vbo->numWFVertices * 3, boxWFVBO->numVertices * 3 * sizeof(float), boxWFVBO->attribs[VERTEX], boxWFVBO->numVertices * 3 * sizeof(float));
				toColorID(index, face);
				for (l = 0; l < (boxVBO->numVertices / 6); ++l)
				{
					for (m = 0; m < 6; ++m)
					{
						memcpy_s(ids + l * 24 + m * 4, 4 * sizeof(float), face, 4 * sizeof(float));
					}
				}
				memcpy_s(vbo->attribs[FACE] + vbo->numVertices * 4, boxVBO->numVertices * 4 * sizeof(float), ids, boxVBO->numVertices * 4 * sizeof(float));
				vbo->numVertices += boxVBO->numVertices;
				vbo->numWFVertices += boxWFVBO->numVertices;
				count++;
				for (l = 0; l < boxVBO->numVertices; ++l)
				{
					for (m = 0; m < 3; ++m)
					{
						if (vbo->bbox->min[m] > boxVBO->attribs[VERTEX][l * 3 + m]) {
							vbo->bbox->min[m] = boxVBO->attribs[VERTEX][l * 3 + m];
						}
						if (vbo->bbox->max[m] < boxVBO->attribs[VERTEX][l * 3 + m]) {
							vbo->bbox->max[m] = boxVBO->attribs[VERTEX][l * 3 + m];
						}
					}
				}
			}
		}
	}
	genBBoxVBO(vbo->bbox);
	//vbo->bbox->min[0] = vbo->bbox->min[1] = vbo->bbox->min[2] = INT_MAX;
	//vbo->bbox->max[0] = vbo->bbox->max[1] = vbo->bbox->max[2] = INT_MIN;
	vbo->enable = true;
	vbo->enableBuffers[VERTEX] =
		vbo->enableBuffers[NORMAL] =
		vbo->enableBuffers[WIREFRAME] =
		vbo->enableBuffers[FACE] = true;
	deleteObject(&boxVBO);
	deleteObject(&boxWFVBO);

	return vbo;
}
vbo_t * createVoxelVBO(vdata_t * vd)
{
	bool neighbor[26];
	int i, j, k, l, m, values[3], index, count, numFaces, resolution[3];
	float translateMat[16], scaleMat[16], ids[144], face[4], origin[3];
	voxel_t* vox = vd->rawdata;
	vbo_t* vbo;
	vbo_t* boxVBO;
	vbo_t* boxWFVBO;

	origin[X] = -128;
	origin[Y] = -128;
	origin[Z] = -128;

	// count drawing faces
	numFaces = 0;
	for (i = 0; i < vd->resolution[Z]; ++i)
	{
		for (j = 0; j < vd->resolution[Y]; ++j)
		{
			for (k = 0; k < vd->resolution[X]; ++k)
			{
				index = k + j * vd->resolution[X] + i * vd->resolution[X] * vd->resolution[Y];
				if (vd->rawdata[index].data == VOX_EMPTY) continue;
				(k + 1 < vd->resolution[X] && vd->rawdata[index].data) ?
					0 : numFaces++;
				(k > 1 && vd->rawdata[index - 1].data) ?
					0 : numFaces++;
				(j + 1 < vd->resolution[Y] && vd->rawdata[index + vd->resolution[X]].data) ?
					0 : numFaces++;
				(j > 1 && vd->rawdata[index - vd->resolution[X]].data) ?
					0 : numFaces++;
				(i + 1 < vd->resolution[Z] && vd->rawdata[index + vd->resolution[X] * vd->resolution[Y]].data) ?
					0 : numFaces++;
				(i > 1 && vd->rawdata[index - vd->resolution[X] * vd->resolution[Y]].data) ?
					0 : numFaces++;
			}
		}
	}


	// creat vbo
	vbo = newObject(NULL);
	strcpy_s(vbo->name, vd->name);
	vbo->attribs[VERTEX] = (float*)calloc(numFaces * 108, sizeof(float));
	vbo->attribs[NORMAL] = (float*)calloc(numFaces * 108, sizeof(float));
	vbo->attribs[WIREFRAME] = (float*)calloc(numFaces * 108, sizeof(float));
	vbo->attribs[FACE] = (float*)calloc(numFaces * 144, sizeof(float));
	boxVBO = newBox(NULL);
	boxWFVBO = newBoxWF(NULL);
	scale(vd->voxelsize[X], vd->voxelsize[Y], vd->voxelsize[Z], scaleMat);
	vbo->numVertices = 0;
	vbo->numWFLoop = 4;

	count = 0;
	for (i = 0; i < vd->resolution[Z]; ++i)
	{
		for (j = 0; j < vd->resolution[Y]; ++j)
		{
			for (k = 0; k < vd->resolution[X]; ++k)
			{
				index = k + j * vd->resolution[X] + i * vd->resolution[X] * vd->resolution[Y];
				if (vd->rawdata[index].data == VOX_EMPTY) continue;
				neighbor[0] = (k + 1 < vd->resolution[X] && vd->rawdata[index + 1].data) ?
					false : true;
				neighbor[1] = (k > 1 && vd->rawdata[index - 1].data) ?
					false : true;
				neighbor[2] = (j + 1 < vd->resolution[Y] && vd->rawdata[index + vd->resolution[X]].data) ?
					false : true;
				neighbor[3] = (j > 1 && vd->rawdata[index - vd->resolution[X]].data) ?
					false : true;
				neighbor[4] = (i + 1 < vd->resolution[Z] && vd->rawdata[index + vd->resolution[X] * vd->resolution[Y]].data) ?
					false : true;
				neighbor[5] = (i > 1 && vd->rawdata[index - vd->resolution[X] * vd->resolution[Y]].data) ?
					false : true;
				initBox(boxVBO, neighbor);
				initBoxWF(boxWFVBO, neighbor);
				translate(origin[0] + k * vd->voxelsize[X],
					origin[1] + j * vd->voxelsize[Y],
					origin[2] + i * vd->voxelsize[Z],
					translateMat);
				multMat4fv(translateMat, scaleMat, boxVBO->modelMat);
				multMat4fv(translateMat, scaleMat, boxWFVBO->modelMat);
				translateVBO(boxVBO, false);
				translateVBO(boxWFVBO, false);
				memcpy_s(vbo->attribs[VERTEX] + vbo->numVertices * 3, boxVBO->numVertices * 3 * sizeof(float), boxVBO->attribs[VERTEX], boxVBO->numVertices * 3 * sizeof(float));
				memcpy_s(vbo->attribs[NORMAL] + vbo->numVertices * 3, boxVBO->numVertices * 3 * sizeof(float), boxVBO->attribs[NORMAL], boxVBO->numVertices * 3 * sizeof(float));
				memcpy_s(vbo->attribs[WIREFRAME] + vbo->numWFVertices * 3, boxWFVBO->numVertices * 3 * sizeof(float), boxWFVBO->attribs[VERTEX], boxWFVBO->numVertices * 3 * sizeof(float));
				toColorID(index, face);
				for (l = 0; l < (boxVBO->numVertices / 6); ++l)
				{
					for (m = 0; m < 6; ++m)
					{
						memcpy_s(ids + l * 24 + m * 4, 4 * sizeof(float), face, 4 * sizeof(float));
					}
				}
				memcpy_s(vbo->attribs[FACE] + vbo->numVertices * 4, boxVBO->numVertices * 4 * sizeof(float), ids, boxVBO->numVertices * 4 * sizeof(float));
				vbo->numVertices += boxVBO->numVertices;
				vbo->numWFVertices += boxWFVBO->numVertices;
				count++;
				for (l = 0; l < boxVBO->numVertices; ++l)
				{
					for (m = 0; m < 3; ++m)
					{
						if (vbo->bbox->min[m] > boxVBO->attribs[VERTEX][l * 3 + m]) {
							vbo->bbox->min[m] = boxVBO->attribs[VERTEX][l * 3 + m];
						}
						if (vbo->bbox->max[m] < boxVBO->attribs[VERTEX][l * 3 + m]) {
							vbo->bbox->max[m] = boxVBO->attribs[VERTEX][l * 3 + m];
						}
					}
				}
			}
		}
	}
	genBBoxVBO(vbo->bbox);
	//vbo->bbox->min[0] = vbo->bbox->min[1] = vbo->bbox->min[2] = INT_MAX;
	//vbo->bbox->max[0] = vbo->bbox->max[1] = vbo->bbox->max[2] = INT_MIN;
	vbo->enable = true;
	/*
	vbo->enableBuffers[VERTEX] =
		vbo->enableBuffers[NORMAL] =
		vbo->enableBuffers[WIREFRAME] =
		vbo->enableBuffers[FACE] = true;
		*/
	vbo->enableBuffers[VERTEX] = true;
	deleteObject(&boxVBO);
	deleteObject(&boxWFVBO);

	return vbo;
}
/*
vbo_t* createVoxelClipPlain(vbo_t* vbo, float clip[4], int voxIdx,  voxelobj_t* obj)
{
	int i, j, k, numVoxels, index, plainIdx;
	static int lastNumVoxels = 0;
	float offset, v0[3], v1[3], v2[3], v3[3], tileV[18], tileN[18], tileC[24], color[4];
	voxel_t* vox;

	if (vbo == NULL)
	{
		vbo = newObject(NULL);

		vbo->enableBuffers[VERTEX] =
		vbo->enableBuffers[NORMAL] =
		vbo->enableBuffers[COLOR] = true;
		vbo->material->isEnable = false;
	}
	vox = obj->voxels[voxIdx];
	// realloc x clip plain
	if (fabs(clip[0]) == 1) {
		numVoxels = vox->resolution[Y] * vox->resolution[Z];
		if (lastNumVoxels == numVoxels) {
			return vbo;
		}
		if (vbo->attribs[VERTEX] != NULL) free(vbo->attribs[VERTEX]);
		if (vbo->attribs[NORMAL] != NULL) free(vbo->attribs[NORMAL]);
		if (vbo->attribs[COLOR] != NULL) free(vbo->attribs[COLOR]);
		vbo->attribs[VERTEX] = (float*)calloc(numVoxels * 18, sizeof(float));
		assert(vbo->attribs[VERTEX] != NULL);
		vbo->attribs[NORMAL] = (float*)calloc(numVoxels * 18, sizeof(float));
		assert(vbo->attribs[NORMAL] != NULL);
		vbo->attribs[COLOR] = (float*)calloc(numVoxels * 24, sizeof(float));
		assert(vbo->attribs[COLOR] != NULL);
	}
	// y clip
	else if (fabs(clip[1]) == 1) {
		numVoxels = vox->resolution[X] * vox->resolution[Z];
		if (lastNumVoxels == numVoxels) {
			return vbo;
		}
		if (vbo->attribs[VERTEX] != NULL) free(vbo->attribs[VERTEX]);
		if (vbo->attribs[NORMAL] != NULL) free(vbo->attribs[NORMAL]);
		if (vbo->attribs[COLOR] != NULL) free(vbo->attribs[COLOR]);
		vbo->attribs[VERTEX] = (float*)calloc(numVoxels * 18, sizeof(float));
		assert(vbo->attribs[VERTEX] != NULL);
		vbo->attribs[NORMAL] = (float*)calloc(numVoxels * 18, sizeof(float));
		assert(vbo->attribs[NORMAL] != NULL);
		vbo->attribs[COLOR] = (float*)calloc(numVoxels * 24, sizeof(float));
		assert(vbo->attribs[COLOR] != NULL);
	}
	// z clip
	else if (fabs(clip[2]) == 1) {
		numVoxels = vox->resolution[X] * vox->resolution[Y];
		if (lastNumVoxels == numVoxels) {
			return vbo;
		}
		if (vbo->attribs[VERTEX] != NULL) free(vbo->attribs[VERTEX]);
		if (vbo->attribs[NORMAL] != NULL) free(vbo->attribs[NORMAL]);
		if (vbo->attribs[COLOR] != NULL) free(vbo->attribs[COLOR]);
		vbo->attribs[VERTEX] = (float*)calloc(numVoxels * 18, sizeof(float));
		assert(vbo->attribs[VERTEX] != NULL);
		vbo->attribs[NORMAL] = (float*)calloc(numVoxels * 18, sizeof(float));
		assert(vbo->attribs[NORMAL] != NULL);
		vbo->attribs[COLOR] = (float*)calloc(numVoxels * 24, sizeof(float));
		assert(vbo->attribs[COLOR] != NULL);
	}
	lastNumVoxels = numVoxels;

	return vbo;
}

void updateVoxelClipPlain(vbo_t* vbo, float clip[4], int voxIdx, voxelobj_t* obj)
{
	int i, j, k, numVoxels, index, plainIdx;
	float offset, v0[3], v1[3], v2[3], v3[3], tileV[18], tileN[18], tileC[24], color[4];
	voxel_t* vox;

	numVoxels = 0;
	vox = obj->voxels[voxIdx];
	offset = clip[3];
	// x clip
	if (fabs(clip[X]) == 1) {
		if (clip[X] > 0) {
			offset *= -1;
		}
		plainIdx = floor((offset - vox->bbox.min[X])  / obj->voxelSize[X]);
		if (plainIdx <= 0 || plainIdx >= vox->resolution[X] - 1) {
			vbo->numVertices = 0;
			return;
		}
		numVoxels = 0;
		for (i = 0; i < vox->resolution[Y]; ++i)
		{
			for (j = 0; j < vox->resolution[Z]; ++j)
			{
				index = plainIdx + i * vox->resolution[X] + j * vox->resolution[X] * vox->resolution[Y];
				if (vox->data[index] != VOX_EMPTY) {
					if (vox->data[index] == VOX_SURFACE) {
						color[0] = 0.6;
						color[1] = 0.1;
						color[2] = 0.1;
						color[3] = 1.0;
					}
					else if (vox->data[index] == VOX_SOLID) {
						color[0] = 0.1;
						color[1] = 0.5;
						color[2] = 0.7;
						color[3] = 1.0;
					}
					else {
						color[0] = 0.0;
						color[1] = 0.0;
						color[2] = 0.0;
						color[3] = 1.0;
					}
					copy3fv(v0, vox->bbox.min);
					copy3fv(v1, vox->bbox.min);
					copy3fv(v2, vox->bbox.min);
					copy3fv(v3, vox->bbox.min);
					v0[0] = offset;	v0[1] += i * obj->voxelSize[Y];			v0[2] += j * obj->voxelSize[Z];
					v1[0] = offset;	v1[1] += i * obj->voxelSize[Y];			v1[2] += (j + 1) * obj->voxelSize[Z];
					v2[0] = offset;	v2[1] += (i + 1) * obj->voxelSize[Y];	v2[2] += (j + 1) * obj->voxelSize[Z];
					v3[0] = offset;	v3[1] += (i + 1) * obj->voxelSize[Y];	v3[2] += j * obj->voxelSize[Z];
					createPolygonVBO(v0, v1, v2, v3, tileV, tileN);
					if (clip[X] > 0) {
						for (k = 0; k < 18; ++k) tileN[k] *= -1;
					}
					for (k = 0; k < 24; k += 4)
					{
						memcpy(vbo->attribs[COLOR] + numVoxels * 24 + k, color, 4 * sizeof(float));
					}
					memcpy(vbo->attribs[VERTEX] + numVoxels * 18, tileV, 18 * sizeof(float));
					memcpy(vbo->attribs[NORMAL] + numVoxels * 18, tileN, 18 * sizeof(float));
					numVoxels++;
				}
			} // end loop j
		} // end loop i
	} // end x clip
	  // y clip
	else if (fabs(clip[Y]) == 1) {
		if (clip[Y] > 0) {
			offset *= -1;
		}
		plainIdx = floor((offset - vox->bbox.min[Y]) / obj->voxelSize[Y]);
		if (plainIdx <= 0 || plainIdx >= vox->resolution[Y] - 1) {
			vbo->numVertices = 0;
			return;
		}
		numVoxels = 0;
		for (i = 0; i < vox->resolution[X]; ++i)
		{
			for (j = 0; j < vox->resolution[Z]; ++j)
			{
				index = i + plainIdx * vox->resolution[X] + j * vox->resolution[X] * vox->resolution[Y];
				if (vox->data[index] != VOX_EMPTY) {
					if (vox->data[index] == VOX_SURFACE) {
						color[0] = 0.6;
						color[1] = 0.1;
						color[2] = 0.1;
						color[3] = 1.0;
					}
					else if (vox->data[index] == VOX_SOLID) {
						color[0] = 0.1;
						color[1] = 0.5;
						color[2] = 0.7;
						color[3] = 1.0;
					}
					else {
						color[0] = 0.0;
						color[1] = 0.0;
						color[2] = 0.0;
						color[3] = 1.0;
					}
					copy3fv(v0, vox->bbox.min);
					copy3fv(v1, vox->bbox.min);
					copy3fv(v2, vox->bbox.min);
					copy3fv(v3, vox->bbox.min);
					v0[0] += i * obj->voxelSize[X];			v0[1] = offset;	v0[2] += j * obj->voxelSize[Z];
					v1[0] += (i + 1) * obj->voxelSize[X];	v1[1] = offset;	v1[2] += j * obj->voxelSize[Z];
					v2[0] += (i + 1) * obj->voxelSize[X];	v2[1] = offset;	v2[2] += (j + 1) * obj->voxelSize[Z];
					v3[0] += i * obj->voxelSize[X];			v3[1] = offset;	v3[2] += (j + 1) * obj->voxelSize[Z];
					createPolygonVBO(v0, v1, v2, v3, tileV, tileN);
					if (clip[Y] > 0) {
						for (k = 0; k < 18; ++k) tileN[k] *= -1;
					}
					for (k = 0; k < 24; k += 4)
					{
						memcpy(vbo->attribs[COLOR] + numVoxels * 24 + k, color, 4 * sizeof(float));
					}
					memcpy(vbo->attribs[VERTEX] + numVoxels * 18, tileV, 18 * sizeof(float));
					memcpy(vbo->attribs[NORMAL] + numVoxels * 18, tileN, 18 * sizeof(float));
					numVoxels++;
				}
			} // end loop j
		} // end loop i
	} // end y clip
	  // z clip
	else if (fabs(clip[Z]) == 1) {
		if (clip[Z] > 0) {
			offset *= -1;
		}
		plainIdx = floor((offset - vox->bbox.min[Z]) / obj->voxelSize[Z]);
		if (plainIdx <= 0 || plainIdx >= vox->resolution[Z] - 1) {
			vbo->numVertices = 0;
			return;
		}
		numVoxels = 0;
		for (i = 0; i < vox->resolution[X]; ++i)
		{
			for (j = 0; j < vox->resolution[Y]; ++j)
			{
				index = i + j * vox->resolution[X] + plainIdx * vox->resolution[X] * vox->resolution[Y];
				if (vox->data[index] != VOX_EMPTY) {
					if (vox->data[index] == VOX_SURFACE) {
						color[0] = 0.6;
						color[1] = 0.1;
						color[2] = 0.1;
						color[3] = 1.0;
					}
					else if (vox->data[index] == VOX_SOLID) {
						color[0] = 0.1;
						color[1] = 0.5;
						color[2] = 0.7;
						color[3] = 1.0;
					}
					else {
						color[0] = 0.0;
						color[1] = 0.0;
						color[2] = 0.0;
						color[3] = 1.0;
					}
					copy3fv(v0, vox->bbox.min);
					copy3fv(v1, vox->bbox.min);
					copy3fv(v2, vox->bbox.min);
					copy3fv(v3, vox->bbox.min);
					v0[0] += i * obj->voxelSize[X];			v0[1] += j * obj->voxelSize[Y];			v0[2] = offset;
					v1[0] += (i + 1) * obj->voxelSize[X];	v1[1] += j * obj->voxelSize[Y];			v1[2] = offset;
					v2[0] += (i + 1) * obj->voxelSize[X];	v2[1] += (j + 1) * obj->voxelSize[Y];	v2[2] = offset;
					v3[0] += i * obj->voxelSize[X];			v3[1] += (j + 1) * obj->voxelSize[Y];	v3[2] = offset;
					createPolygonVBO(v0, v1, v2, v3, tileV, tileN);
					if (clip[Z] > 0) {
						for (k = 0; k < 18; ++k) tileN[k] *= -1;
					}
					for (k = 0; k < 24; k += 4)
					{
						memcpy(vbo->attribs[COLOR] + numVoxels * 24 + k, color, 4 * sizeof(float));
					}
					memcpy(vbo->attribs[VERTEX] + numVoxels * 18, tileV, 18 * sizeof(float));
					memcpy(vbo->attribs[NORMAL] + numVoxels * 18, tileN, 18 * sizeof(float));
					numVoxels++;
				}
			} // end loop j
		} // end loop i
	} // end z clip
	vbo->numVertices = numVoxels * 6;
}*/