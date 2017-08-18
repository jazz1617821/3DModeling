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

#include <stddef.h>
#include "enqlist.h"

typedef struct Voxel
{
	unsigned char data;
}voxel_t;

typedef struct VoxelData
{
	char* name = NULL;			//Data Name
	int resolution[3];			//Data resolution	
	float voxelSize[3];			//Data voxle size
	bool isBitCompress;			//Data store type
	voxel_t* rawData = NULL;	//Raw data
}vdata_t;

typedef struct AxisAlignedBoundingBox {
	float minBound[3];			//order x y z
	float maxBound[3];			//order x y z
}aabb_t;

struct DataItem;

typedef struct VoxelObject
{
	//VoxelObject
	char* name = NULL;					//Object name
	aabb_t* bbox = NULL;				//Axis Aligned Bounding Box
	int numberOfChild;					//Number of child
	VoxelObject* parent = NULL;			//Up stair voxel object pointer
	VoxelObject* nextSibling = NULL;	//Same stair next voxel object pointer
	VoxelObject* prevSibling = NULL;	//Same stair previous voxel object pointer

	VoxelObject* firstChild = NULL;		//Down stair first child voxel object pointer, leaf voxel object does not have this.
	DataItem* dataItem = NULL;			//
}vobj_t;

typedef struct DataItem
{
	enqdlist_t enqDList;				//Enqueueable double linked list 
	VoxelObject* parent = NULL;			//The leaf object which has this dataitem
	VoxelData* voxlData = NULL;			//Voxel data pointer
}ditem_t;

typedef struct VoxelModel
{
	char* name = NULL;					//Model name
	int resolution[3];					//Max Resolution
	float voxelSize[3];					//Unite all data to one voxel size
	int numberOfVoxelData;				//Raw data count

	VoxelObject* rootObj = NULL;		//Root of the object tree
	DataItem* rootItem = NULL;			//Root of the data item 
}vmodel_t;

#endif //_VOXEL_H