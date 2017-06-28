#include "voxel.h"
#include "mymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>


#define X 0
#define Y 1
#define Z 2


/////////////////////////////
//     gloabal variable    //
/////////////////////////////
int g_voxelBvhID = 0;
int g_voxelID = 0;

int g_bound = 2;


// x->y->z increament
const int g_nbMat[26][3] = 
{
	{ -1, -1, -1 },{ 0, -1, -1 },{ 1, -1, -1 },
	{ -1,  0, -1 },{ 0,  0, -1 },{ 1,  0, -1 },
	{ -1,  1, -1 },{ 0,  1, -1 },{ 1,  1, -1 },
	{ -1, -1,  0 },{ 0, -1,  0 },{ 1, -1,  0 },
	{ -1,  0,  0 },				 { 1,  0,  0 },
	{ -1,  1,  0 },{ 0,  1,  0 },{ 1,  1,  0 },
	{ -1, -1,  1 },{ 0, -1,  1 },{ 1, -1,  1 },
	{ -1,  0,  1 },{ 0,  0,  1 },{ 1,  0,  1 },
	{ -1,  1,  1 },{ 0,  1,  1 },{ 1,  1,  1 } 
};


const int g_cityMat[6][3] =
{
	{  0,  0, -1 },
	{  0, -1,  0 },
	{ -1,  0,  0 },
	{  1,  0,  0 },
	{  0,  1,  0 },
	{  0,  0,  1 }
};

static voxel_t* modelVoxelize(model_t* model, float origin[3], float voxelSize[3]);
static voxelbvh_t* buildVoxelBVH(modelbvh_t* root, int tableSize, mvt_t* table);
static voxelbvh_t* deleteBVH(voxelbvh_t* root);


static void findMinMax(float x0, float x1, float x2, float* valueMin, float* valueMax);
static int planeBoxOverlap(float normal[3], float vert[3], float maxbox[3]);
static bool axis_x01(const float a, const float b,
					 const float fa, const float fb,
					 const float v0[3], const float v2[3],
					 const float boxHS[3], const float th);
static bool axis_x2(const float a, const float b,
					const float fa, const float fb,
					const float v0[3], const float v1[3],
					const float boxHS[3], const float th);
static bool axis_y02(const float a, const float b,
					 const float fa, const float fb,
					 const float v0[3], const float v2[3],
					 const float boxHS[3], const float th);
static bool axis_y1(const float a, const float b,
					const float fa, const float fb,
					const float v0[3], const float v1[3],
					const float boxHS[3], const float th);
static bool axis_z12(const float a, const float b,
					 const float fa, const float fb,
					 const float v1[3], const float v2[3],
					 const float boxHS[3], const float th);
static bool axis_z0(const float a, const float b,
					const float fa, const float fb,
					const float v0[3], const float v1[3],
					const float boxHS[3], const float th);
static bool triBoxOverlap(float boxCenter[3], float boxHS[3], float v[9]);


static void findMinMax(float v0, float v1, float v2, float* valueMin, float* valueMax)
{
	*valueMin = *valueMax = v0;
	if (v1 < *valueMin) *valueMin = v1;
	if (v1 > *valueMax) *valueMax = v1;
	if (v2 < *valueMin) *valueMin = v2;
	if (v2 > *valueMax) *valueMax = v2;
}

static int planeBoxOverlap(float normal[3], float vert[3], float maxbox[3])
{
	int i;
	float vmin[3], vmax[3], v;

	for (i = 0; i <= 3; i++)
	{
		v = vert[i];
		if (normal[i] > 0.0f) {
			vmin[i] = -maxbox[i] - v;
			vmax[i] = maxbox[i] - v;
		}
		else {
			vmin[i] = maxbox[i] - v;
			vmax[i] = -maxbox[i] - v;
		}
	}
	if (dot3fv(normal, vmin) > 0.0f) return 0;
	if (dot3fv(normal, vmax) >= 0.0f) return 1;
	return 0;
}

/*======================== X-tests ========================*/

static bool axis_x01(const float a, const float b,
	const float fa, const float fb,
	const float v0[3], const float v2[3],
	const float boxHS[3], const float th)
{
	float p0, p2, max, min, rad;

	p0 = a * v0[1] - b * v0[2];
	p2 = a * v2[1] - b * v2[2];
	if (p0 < p2) {
		min = p0;
		max = p2;
	}
	else {
		min = p2;
		max = p0;
	}
	rad = fa * boxHS[1] + fb * boxHS[2];
	if (min > rad + th || max < -rad - th)
		return false;
	return true;
}

static bool axis_x2(const float a, const float b,
	const float fa, const float fb,
	const float v0[3], const float v1[3],
	const float boxHS[3], const float th)
{
	float p0, p1, max, min, rad;

	p0 = a * v0[1] - b * v0[2];
	p1 = a * v1[1] - b * v1[2];
	if (p0 < p1) {
		min = p0;
		max = p1;
	}
	else {
		min = p1;
		max = p0;
	}
	rad = fa * boxHS[1] + fb * boxHS[2];
	if (min > rad + th || max < -rad - th)
		return false;
	return true;
}

/*======================== Y-tests ========================*/

static bool axis_y02(const float a, const float b,
	const float fa, const float fb,
	const float v0[3], const float v2[3],
	const float boxHS[3], const float th)
{
	float p0, p2, max, min, rad;

	p0 = -a * v0[0] + b * v0[2];
	p2 = -a * v2[0] + b * v2[2];
	if (p0 < p2) {
		min = p0;
		max = p2;
	}
	else {
		min = p2;
		max = p0;
	}
	rad = fa * boxHS[0] + fb * boxHS[2];
	if (min > rad + th || max < -rad - th)
		return false;
	return true;
}

static bool axis_y1(const float a, const float b,
	const float fa, const float fb,
	const float v0[3], const float v1[3],
	const float boxHS[3], const float th)
{
	float p0, p1, max, min, rad;

	p0 = -a * v0[0] + b * v0[2];
	p1 = -a * v1[0] + b * v1[2];
	if (p0 < p1) {
		min = p0;
		max = p1;
	}
	else {
		min = p1;
		max = p0;
	}
	rad = fa * boxHS[0] + fb * boxHS[2];
	if (min > rad + th || max < -rad - th)
		return false;
	return true;
}



/*======================== Z-tests ========================*/

static bool axis_z12(const float a, const float b,
	const float fa, const float fb,
	const float v1[3], const float v2[3],
	const float boxHS[3], const float th)
{
	float p1, p2, max, min, rad;

	p1 = a * v1[0] - b * v1[1];
	p2 = a * v2[0] - b * v2[1];
	if (p2 < p1) {
		min = p2;
		max = p1;
	}
	else {
		min = p1;
		max = p2;
	}
	rad = fa * boxHS[0] + fb * boxHS[1];
	if (min > rad + th || max < -rad - th)
		return false;
	return true;
}

static bool axis_z0(const float a, const float b,
	const float fa, const float fb,
	const float v0[3], const float v1[3],
	const float boxHS[3], const float th)
{
	float p0, p1, max, min, rad;

	p0 = a * v0[0] - b * v0[1];
	p1 = a * v1[0] - b * v1[1];
	if (p0 < p1) {
		min = p0;
		max = p1;
	}
	else {
		min = p1;
		max = p0;
	}
	rad = fa * boxHS[0] + fb * boxHS[1];
	if (min > rad + th || max < -rad - th)
		return false;
	return true;
}

bool triBoxOverlap(float boxCenter[3], float boxHS[3], float v[9])
{
	float v0[3];
	float v1[3];
	float v2[3];
	float e0[3];
	float e1[3];
	float e2[3];
	float min, max, fex, fey, fez;
	float th = 0.00001;
	float normal[3];

	subtract3fv((v), boxCenter, v0);
	subtract3fv((v + 3), boxCenter, v1);
	subtract3fv((v + 6), boxCenter, v2);

	subtract3fv(v1, v0, e0);
	subtract3fv(v2, v1, e1);
	subtract3fv(v0, v2, e2);

	fex = fabs(e0[0]);
	fey = fabs(e0[1]);
	fez = fabs(e0[2]);

	if (!axis_x01(e0[2], e0[1], fez, fey, v0, v2, boxHS, th)) return false;
	if (!axis_y02(e0[2], e0[0], fez, fex, v0, v2, boxHS, th)) return false;
	if (!axis_z12(e0[1], e0[0], fey, fex, v1, v2, boxHS, th)) return false;

	fex = fabs(e1[0]);
	fey = fabs(e1[1]);
	fez = fabs(e1[2]);

	if (!axis_x01(e1[2], e1[1], fez, fey, v0, v2, boxHS, th)) return false;
	if (!axis_y02(e1[2], e1[0], fez, fex, v0, v2, boxHS, th)) return false;
	if (!axis_z0(e1[1], e1[0], fey, fex, v0, v1, boxHS, th)) return false;

	fex = fabs(e2[0]);
	fey = fabs(e2[1]);
	fez = fabs(e2[2]);

	if (!axis_x2(e2[2], e2[1], fez, fey, v0, v1, boxHS, th)) return false;
	if (!axis_y1(e2[2], e2[0], fez, fex, v0, v1, boxHS, th)) return false;
	if (!axis_z12(e2[1], e2[0], fey, fex, v1, v2, boxHS, th)) return false;
	
	// test in X-direction
	findMinMax(v0[0], v1[0], v2[0], &min, &max);
	if(min > boxHS[0] || max < -boxHS[0]) return 0;

	// test in Y-direction
	findMinMax(v0[1], v1[1], v2[1], &min, &max);
	if(min > boxHS[1] || max < -boxHS[1]) return 0;

	// test in Z-direction
	findMinMax(v0[2], v1[2], v2[2], &min, &max);
	if (min > boxHS[2] || max < -boxHS[2]) return 0;

	cross3fv(e0, e1, normal);
	if (!planeBoxOverlap(normal, v0, boxHS)) return 0;
	
	return true;
}

static voxel_t* modelVoxelize(model_t* model, float origin[3], float voxelSize[3])
{
	int i, j, k, l, m, n, o, index, resolution[3];
	float boxCenter[3], boxHalf[3], triVert[9], minBound[3], maxBound[3], start[3], end[3], values[3];
	voxel_t* voxel;

	// initialize
	voxel = (voxel_t*)calloc(1, sizeof(voxel_t));
	memcpy(voxel->voxelSize, voxelSize, 3 * sizeof(float));
	for (i = 0; i < 3; ++i)
	{
		voxel->minBound[i] = ceil((model->bbox.min[i] - origin[i]) / voxelSize[i]) - g_bound;
		voxel->maxBound[i] = ceil((model->bbox.max[i] - origin[i]) / voxelSize[i]) + g_bound;
		voxel->resolution[i] = voxel->maxBound[i] - voxel->minBound[i];
		resolution[i] = voxel->resolution[i];

		boxHalf[i] = voxelSize[i] / 2.0;
	}
	voxel->data = (unsigned char*)calloc(resolution[0] *
										 resolution[1] *
										 resolution[2],
										 sizeof(unsigned char));

	for (i = 0; i < 3; ++i)
	{
		voxel->bbox.max[i] = (model->bbox.max[i] + model->bbox.min[i] + resolution[i] * voxelSize[i]) / 2.0;
		voxel->bbox.min[i] = (model->bbox.max[i] + model->bbox.min[i] - resolution[i] * voxelSize[i]) / 2.0;
	}

	voxel->numVoxels = 0;
	for (i = 0; i < model->numFaces; ++i)
	{
		getTriangle(model, i, triVert);
		minBound[X] = minBound[Y] = minBound[Z] = INT_MAX;
		maxBound[X] = maxBound[Y] = maxBound[Z] = INT_MIN;
		// find bound of triangle
		for (j = 0; j < 3; ++j)
		{
			for (k = 0; k < 3; ++k)
			{
				if (minBound[k] > triVert[j * 3 + k]) {
					minBound[k] = triVert[j * 3 + k];
				}
				if (maxBound[k] < triVert[j * 3 + k]) {
					maxBound[k] = triVert[j * 3 + k];
				}
			}
		}

		start[0] = floor(fabs(minBound[0] - origin[0]) / voxelSize[X]);
		start[1] = floor(fabs(minBound[1] - origin[1]) / voxelSize[Y]);
		start[2] = floor(fabs(minBound[2] - origin[2]) / voxelSize[Z]);
		end[0] = ceil(fabs(maxBound[0] - origin[0]) / voxelSize[X]);
		end[1] = ceil(fabs(maxBound[1] - origin[1]) / voxelSize[Y]);
		end[2] = ceil(fabs(maxBound[2] - origin[2]) / voxelSize[Z]);

		// voxelize
		for (j = start[0]; j <= end[0]; ++j)
		{
			for (k = start[1]; k <= end[1]; ++k)
			{
				for (l = start[2]; l <= end[2]; ++l)
				{
					index = j + k * resolution[X] + l * resolution[X] * resolution[Y];
					boxCenter[0] = origin[0] + (j + 0.5) * voxelSize[X];
					boxCenter[1] = origin[1] + (k + 0.5) * voxelSize[Y];
					boxCenter[2] = origin[2] + (l + 0.5) * voxelSize[Z];
					if (triBoxOverlap(boxCenter, boxHalf, triVert)) {
						if (voxel->data[index] == 0) {
							voxel->data[index] = 1;
							voxel->numVoxels++;
						}
					} // tri box overlap testing
				} // end loop l
			} // end loop k
		} // end loop j
	} // end triangle loop

	return voxel;
}

static voxelbvh_t* buildVoxelBVH(modelbvh_t* root, int tableSize, mvt_t* table)
{
	int i, j;
	voxelbvh_t* pBvh;

	if (root->numChildren >= 0) {
		pBvh = (voxelbvh_t*)calloc(1, sizeof(voxelbvh_t));
		pBvh->numChildren = root->numChildren;
		strcpy(pBvh->name, root->name);
		for (i = 0; i < root->numChildren; ++i)
		{
			pBvh->children[i] = buildVoxelBVH(root->children[i], tableSize, table);
		}
		// cal bounding box
		initAABB(&pBvh->bbox);
		for (i = 0; i < root->numChildren; ++i)
		{
			for (j = 0; j < 3; ++j)
			{
				if (pBvh->bbox.max[j] < pBvh->children[i]->bbox.max[j]) {
					pBvh->bbox.max[j] = pBvh->children[i]->bbox.max[j];
				}
				if (pBvh->bbox.min[j] > pBvh->children[i]->bbox.min[j]) {
					pBvh->bbox.min[j] = pBvh->children[i]->bbox.min[j];
				}
			}
		}
		return pBvh;
	}
	else {
		pBvh = (voxelbvh_t*)calloc(1, sizeof(voxelbvh_t));
		for (i = 0; i < tableSize; ++i)
		{
			if (table[i].model == root->model) {
				pBvh->voxel = table[i].voxel;
				strcpy(pBvh->name, root->name);
				return pBvh;
			}
		}
	}
	return NULL;
}

voxelobj_t* baseVoxelize(modelobj_t* modobj, float voxelSize[3])
{
	int i, j, k, resolution[3], start[3], end[3], tmp;
	float boxCenter[3], halfSize[3], origin[3];
	char path[1024];
	modelbvh_t* modbvh;
	voxelobj_t* voxobj;
	voxelbvh_t* voxbvh;
	voxel_t* voxel;
	mvt_t* table;


	modbvh = modobj->root;
	
	//initialize
	voxobj = (voxelobj_t*)calloc(1, sizeof(voxelobj_t));
	voxobj->numVoxels = modobj->numModels;
	voxobj->voxels = (voxel_t**)calloc(voxobj->numVoxels, sizeof(voxelobj_t*));

	table = (mvt_t*)calloc(voxobj->numVoxels, sizeof(mvt_t));

	// volume resolution overall
	resolution[0] = ceil((modbvh->bbox.max[0] - modbvh->bbox.min[0]) / voxelSize[X]) + (g_bound * 2);
	resolution[1] = ceil((modbvh->bbox.max[1] - modbvh->bbox.min[1]) / voxelSize[Y]) + (g_bound * 2);
	resolution[2] = ceil((modbvh->bbox.max[2] - modbvh->bbox.min[2]) / voxelSize[Z]) + (g_bound * 2);

	boxCenter[0] = (modbvh->bbox.min[0] + modbvh->bbox.max[0]) / 2.0;
	boxCenter[1] = (modbvh->bbox.min[1] + modbvh->bbox.max[1]) / 2.0;
	boxCenter[2] = (modbvh->bbox.min[2] + modbvh->bbox.max[2]) / 2.0;

	origin[0] = boxCenter[0] - (resolution[0] * voxelSize[X]) / 2.0;
	origin[1] = boxCenter[1] - (resolution[1] * voxelSize[Y]) / 2.0;
	origin[2] = boxCenter[2] - (resolution[2] * voxelSize[Z]) / 2.0;

	voxobj->voxelSize[X] = voxelSize[X];
	voxobj->voxelSize[Y] = voxelSize[Y];
	voxobj->voxelSize[Z] = voxelSize[Z];

	for (i = 0; i < 3; ++i)
	{
		voxobj->resolution[i] = resolution[i];
	}

	// voxelize each model
	for (i = 0; i < modobj->numModels; ++i)
	{
		voxobj->voxels[i] = modelVoxelize(modobj->models[i], origin, voxelSize);
		strcpy(voxobj->voxels[i]->name, modobj->models[i]->name);
		voxobj->voxels[i]->id = g_voxelID++;
		table->model = modobj->models[i];
		table->voxel = voxobj->voxels[i];
	}

	// build bvh
	voxbvh = buildVoxelBVH(modbvh, modobj->numModels, table);

	voxobj->root = voxbvh;
	strcpy(voxobj->name, modobj->name);

	return voxobj;
}

bool testWatertight(int x, int y, int z, voxel_t* vox)
{
	int i, j, k, nb, index, offset[3];
	bool nbTest[26];


	for (nb = 0; nb < 26; ++nb)
	{
		nbTest[nb] = false;
		offset[X] = g_nbMat[nb][X];
		offset[Y] = g_nbMat[nb][Y];
		offset[Z] = g_nbMat[nb][Z];
		i = x + offset[X];
		j = y + offset[Y];
		k = z + offset[Z];
		while (i >= 0 && i < vox->resolution[X] &&
			   j >= 0 && j < vox->resolution[Y] &&
			   k >= 0 && k < vox->resolution[Z])
		{
			index = i + j * vox->resolution[X] + k * vox->resolution[X] * vox->resolution[Y];
			if (vox->data[index] == VOX_SURFACE) {
				nbTest[nb] = true;
				break;
			}
			i += offset[X];
			j += offset[Y];
			k += offset[Z];
		}
	} // end loopp nb
	for (nb = 0; nb < 26; ++nb)
	{
		if (nbTest[nb] == false)
			break;
	}
	if (nb == 26) {
		return true;
	}
	return false;
}

void voxRegionGrow(int x, int y, int z, voxel_t* vox)
{
	int i, j, k, l, city, curIdx, nbIdx, offset[3], total;
	bool update;

	total = vox->resolution[X] * vox->resolution[Y] * vox->resolution[Z];
	curIdx = x + y * vox->resolution[X] + z * vox->resolution[X] * vox->resolution[Y];
	vox->data[curIdx] = VOX_SOLID;
	update = true;
	int num = 0;
	while (update)
	{
		update = false;
		for (i = 1; i < vox->resolution[Z] - 1; ++i)
		{
			for (j = 1; j < vox->resolution[Y] - 1; ++j)
			{
				for (k = 1; k < vox->resolution[X] - 1; ++k)
				{
					curIdx = k + j * vox->resolution[X] + i * vox->resolution[X] * vox->resolution[Y];
					if (vox->data[curIdx] == VOX_EMPTY) {
						for (city = 0; city < 6; ++city)
						{
							offset[X] = g_cityMat[city][X]; offset[Y] = g_cityMat[city][Y]; offset[Z] = g_cityMat[city][Z];
							nbIdx = (k + offset[X]) +
								(j + offset[Y]) * vox->resolution[X] +
								(i + offset[Z]) * vox->resolution[X] * vox->resolution[Y];
							if (vox->data[nbIdx] == VOX_SOLID) {
								vox->data[curIdx] = VOX_SOLID;
								num++;
								update = true;
								break;
							}
						} // end loop city
					}
				} // end loop k
			} // end loop j
		} // end loop i
	} // end while loop
	printf("num: %d\n", num);
}

void solidifyVoxel(voxel_t* vox)
{
	int i, j, k, index;

	// select growing point
	for (i = 0; i < vox->resolution[Z] - 1; ++i)
	{
		for (j = 0; j < vox->resolution[Y]; ++j)
		{
			for (k = 0; k < vox->resolution[X]; ++k)
			{
				index = k + j * vox->resolution[X] + i * vox->resolution[X] * vox->resolution[Y];
				if (vox->data[index] == VOX_SURFACE &&
					vox->data[index + vox->resolution[X] * vox->resolution[Y]] == VOX_EMPTY) {
					if (testWatertight(k, j, i + 1, vox)) {
						voxRegionGrow(k, j, i + 1, vox);
					}
				}
			} // end loop k
		} // end loop j
	} // end loop i

	int solid = 0;
	for (i = 0; i < vox->resolution[Z]; ++i)
	{
		for (j = 0; j < vox->resolution[Y]; ++j)
		{
			for (k = 0; k < vox->resolution[X]; ++k)
			{
				index = k + j * vox->resolution[X] + i * vox->resolution[X] * vox->resolution[Y];
				if (vox->data[index] == VOX_SOLID) {
					solid++;
					//vox->data[index] = VOX_SURFACE;
				}
			}
		}
	}
	printf("solid: %d\n", solid);
}

void solidifyVoxelObject(voxelobj_t* obj)
{
	int i;

	for(i = 0; i < obj->numVoxels; ++i)
	{
		solidifyVoxel(obj->voxels[i]);
	}
}

void addVoxelObject(voxelobj_t* obj, voa_t* voa)
{
	// realloc
	if (voa->numObjects >= voa->capObjects) {
		voa->capObjects *= 2;
		voa->objects =
			(voxelobj_t**)realloc(voa->objects,
				voa->capObjects * sizeof(voxelobj_t*));
	}
	// add
	voa->objects[voa->numObjects] = obj;
	voa->numObjects++;
}

voa_t* newVoxelObjectArray()
{
	voa_t* voa;

	voa = (voa_t*)calloc(1, sizeof(voa_t));
	voa->capObjects = 1;
	voa->objects = (voxelobj_t**)calloc(1, sizeof(voxelobj_t*));

	return voa;
}

static voxelbvh_t* deleteBVH(voxelbvh_t* root)
{
	int i;

	if (root->children != NULL) {
		for (i = 0; i < root->numChildren; ++i)
		{
			root->children[i] = deleteBVH(root->children[i]);
		}
	}
	// delete
	if (root->voxel != NULL) {
		free(root->voxel);
	}
	free(root);
	return NULL;
}

void deleteVoxelObjectArray(voa_t** voa)
{
	int i;
	voxelobj_t* obj;

	for (i = 0; i < (*voa)->numObjects; ++i)
	{
		obj = (*voa)->objects[i];
		free(obj->voxels);
		obj->voxels = NULL;
		obj->root = deleteBVH(obj->root);
		free(obj);
	}
	free(*voa);
	*voa = NULL;
}

voxelobj_t* readVOFile(const char* fileName)
{
	int i, j, ret;
	float origin[3], center[3];
	FILE* fp;
	char buf[1024] = { '\0' }, path[1024] = { '\0' }, voxelName[1024] = { '\0' };
	const char* ptr;
	const char* pName;
	voxelobj_t* obj;

	obj = (voxelobj_t*)calloc(1, sizeof(voxelobj_t));

	fp = fopen(fileName, "r");

	// get path
	strcpy(buf, fileName);
	ptr = strrchr(buf, '/');
	strcpy(voxelName, ptr + 1);
	pName = strtok(voxelName, ".");
	strcpy(obj->name, pName);
	strncpy(path, buf, (int)(ptr - buf + 1));

	// parse .vo
	fscanf(fp, "VoxelSize %f %f %f\n", &obj->voxelSize[X],
									   &obj->voxelSize[Y],
									   &obj->voxelSize[Z]);
	fscanf(fp, "Resolution %d*%d*%d\n", &obj->resolution[X],
										&obj->resolution[Y],
										&obj->resolution[Z]);
	fscanf(fp, "Amount %d\n", &obj->numVoxels);
	fscanf(fp, "VoxelData\n");

	origin[0] = -obj->resolution[0] * obj->voxelSize[X] / 2.0;
	origin[1] = 0;
	origin[2] = -obj->resolution[2] * obj->voxelSize[Z] / 2.0;

	obj->voxels = (voxel_t**)calloc(obj->numVoxels, sizeof(voxel_t*));
	for (i = 0; i < obj->numVoxels; ++i)
	{
		fscanf(fp, "%1024[^\n]", voxelName);
		strcpy(buf, path);
		strcat(buf, voxelName);
		obj->voxels[i] = readBinaryVoxel(buf);
		for (j = 0; j < 3; ++j)
		{
			obj->voxels[i]->bbox.min[j] = origin[j] + obj->voxels[i]->minBound[j] * obj->voxelSize[j];
			obj->voxels[i]->bbox.max[j] = origin[j] + obj->voxels[i]->maxBound[j] * obj->voxelSize[j];
		}
	}

	fclose(fp);

	return obj;
}

void writeVOFile(voxelobj_t* obj)
{
	int i, j, voxRes[3];
	char path[1024], fileName[1024];
	FILE* fp;

	// get project path
	GetCurrentDirectoryA(1024, path);
	strcat(path, "\\Voxels\\");
	if (GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(path, 0);
	}
	strcat(path, obj->root->name);
	strcat(path, "\\");
	// create folder
	CreateDirectoryA(path, NULL);

	strcpy(fileName, path);
	strcat(fileName, obj->root->name);
	strcat(fileName, ".vo");
	fp = fopen(fileName, "w");
	if (fp == NULL) {
		fprintf(stderr, "Fail to open %s...\n", fileName);
	}
	// write voxel object file
	// format:
	//		VoxelSize float
	//		Resolution int*int*int
	//		Amount int
	//		VoxelData
	//		[Voxel Name]
	//			...
	//			...
	//		
	fprintf(fp, "VoxelSize %f %f %f\n", obj->voxelSize[X],
										obj->voxelSize[Y],
										obj->voxelSize[Z]);
	fprintf(fp, "Resolution %d*%d*%d\n", obj->resolution[X],
										 obj->resolution[Y],
										 obj->resolution[Z]);
	fprintf(fp, "Amount %d\n", obj->numVoxels);
	fprintf(fp, "VoxelData\n");
	for (i = 0; i < obj->numVoxels; ++i)
	{
		fprintf(fp, "%s.binvox\n", obj->voxels[i]->name);
	}
	// BVH... I will finished later :(

	// end BVH
	fclose(fp);

	// write voxel data
	for (i = 0; i < obj->numVoxels; ++i)
	{
		writeBinaryVoxel(obj->voxels[i], path);
	}
}

voxel_t* readBinaryVoxel(const char* fileName)
{
	FILE *fp;
	const char* ptr;
	voxel_t* vox;

	vox = (voxel_t*)calloc(1, sizeof(voxel_t));
	ptr = strrchr(fileName, '/');
	strcpy(vox->name, ptr + 1);
	fp = fopen(fileName, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Fail to open file...\n");
	}
	fread(&vox->numVoxels, sizeof(int), 1, fp);
	fread(&vox->minBound, sizeof(int), 3, fp);
	fread(&vox->maxBound, sizeof(int), 3, fp);
	fread(&vox->resolution, sizeof(int), 3, fp);
	fread(&vox->voxelSize, sizeof(float), 3, fp);
	vox->data = (unsigned char*)calloc(sizeof(unsigned char), vox->resolution[X] * vox->resolution[Y] * vox->resolution[Z]);
	fread(vox->data, sizeof(unsigned char), vox->resolution[X] * vox->resolution[Y] * vox->resolution[Z], fp);
	fclose(fp);

	return vox;
}

void writeBinaryVoxel(voxel_t* vox, const char* path)
{
	FILE *fp;
	char fileName[1024];

	strcpy(fileName, path);
	strcat(fileName, vox->name);
	strcat(fileName, ".binvox");
	fp = fopen(fileName, "wb");
	fwrite(&vox->numVoxels, sizeof(int), 1, fp);
	fwrite(&vox->minBound, sizeof(int), 3, fp);
	fwrite(&vox->maxBound, sizeof(int), 3, fp);
	fwrite(&vox->resolution, sizeof(int), 3, fp);
	fwrite(&vox->voxelSize, sizeof(float), 3, fp);
	fwrite(vox->data, sizeof(unsigned char), vox->resolution[X] * vox->resolution[Y] * vox->resolution[Z], fp);
	fclose(fp);
}

int testRead(const char* fileName)
{
	FILE* fp;
	int i, num, res[3];
	unsigned char* data;

	res[0] = 58;
	res[1] = 90;
	res[2] = 32;
	data = (unsigned char*)calloc(res[0] * res[1] * res[2], sizeof(unsigned char));
	fp = fopen(fileName, "rb");
	fread(data, sizeof(unsigned char), res[0] * res[1] * res[2], fp);
	num = 0;
	for (i = 0; i < res[0] * res[1] * res[2]; ++i)
	{
		if (data[i] == 1) {
			num++;
		}
	}
	printf("%d\n", num);
	return num;
}

int voxIndex(voxel_t* vox, int x, int y, int z)
{
	return x +
			y * vox->resolution[X] +
			z * vox->resolution[X] * vox->resolution[Y];
}

void countVoxels(voxel_t* vox)
{
	int i, total;

	total = vox->resolution[X] * vox->resolution[Y] * vox->resolution[Z];
	vox->numVoxels = 0;
	for (i = 0; i < total; ++i)
	{
		if (vox->data[i] != VOX_EMPTY)
			vox->numVoxels++;
	}
}