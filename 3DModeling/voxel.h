#ifndef _VOXEL_H
#define _VOXEL_H

#define VOX_X_SIZE 0
#define VOX_Y_SIZE 1
#define VOX_Z_SIZE 2
#define VOX_X_RESOLUTION 0
#define VOX_Y_RESOLUTION 1
#define VOX_Z_RESOLUTION 2

#define VOX_EMPTY 0
#define VOX_SURFACE 1
#define VOX_SOLID 2
#define VOX_IN_THICK 3
#define VOX_OUT_THICK 4

#include "meshmodel.h"

typedef struct VoxelData
{
	char name[512];
	unsigned short id;
	unsigned char* data;
	bool isBitCompress;
	int minBound[3];
	int maxBound[3];
	int resolution[3];
	float voxelSize[3];
	int numVoxels;
	aabb_t bbox;
}voxel_t;

typedef struct VoxelDataBVH
{
	char name[512];
	unsigned short id;

	int numChildren;
	VoxelDataBVH* parent;
	VoxelDataBVH** children;
	voxel_t* voxel;
	aabb_t bbox;
}voxelbvh_t;

typedef struct VoxelObject
{
	char name[512];
	int numVoxels;
	voxel_t** voxels;
	float voxelSize[3];
	int resolution[3];
	voxelbvh_t* root;
}voxelobj_t;

typedef struct VoxelObjectArray
{
	int numObjects;
	int capObjects;
	voxelobj_t** objects;
}voa_t;

typedef struct ModelVoxelTable
{
	model_t* model;
	voxel_t* voxel;
}mvt_t;

voa_t* newVoxelObjectArray();
void deleteVoxelObjectArray(voa_t** voa);

void addVoxelObject(voxelobj_t* obj, voa_t* voa);
voxelobj_t* baseVoxelize(modelobj_t* modobj, float voxelSize[3]);
bool testWatertight(int x, int y, int z, voxel_t* vox);
void voxRegionGrow(int x, int y, int z, voxel_t* vox);
void solidifyVoxel(voxel_t* vox);
void solidifyVoxelObject(voxelobj_t* obj);

voxelobj_t* readVOFile(const char* fileName);
void writeVOFile(voxelobj_t* obj);
voxel_t* readBinaryVoxel(const char* fileName);
void writeBinaryVoxel(voxel_t* vox, const char* path);
int testRead(const char* fileName);
int voxIndex(voxel_t* vox, int x, int y, int z);
void countVoxels(voxel_t* vox);

#endif //_VOXEL_H
