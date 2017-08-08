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

#include "enqlist.h"

typedef struct Voxel
{
	unsigned char data;
}voxel_t;

typedef struct VoxelData
{
	char* name;				//Data Name
	int resolution[3];		//Data resolution	
	float voxelSize[3];		//Data voxle size
	bool isBitCompress;		//Data store type
	voxel_t* rawData;		//Raw data
}vdata_t;

typedef struct DataItem
{
	enqdlist_t enqDList;	//Enqueueable double linked list 
	vobj_t* parent;			//The leaf object which has this dataitem
	vdata_t* voxlData;		//Voxel data pointer
}ditem_t;

typedef struct VoxelObject
{
	//VoxelObject
	char* name;				//Object name
	aabb_t* bbox;			//Axis Aligned Bounding Box
	int numberOfChild;		//Number of child
	vobj_t* parent;			//Up stair voxel object pointer
	vobj_t* nextSibling;	//Same stair next voxel object pointer
	vobj_t* prevSibling;	//Same stair previous voxel object pointer

	vobj_t* firstChild;		//Down stair first child voxel object pointer, leaf voxel object does not have this.
	ditem_t* dataItem;		//
}vobj_t;

typedef struct VoxelModel
{
	char* name;				//Model name
	int resolution[3];		//Max Resolution
	float voxelSize[3];		//Unite all data to one voxel size
	int numberOfVoxelData;	//Raw data count

	VoxelObject* rootObj;	//Root of the object tree
	ditem_t* rootItem;		//Root of the data item 
}vmodel_t;

typedef struct AxisAlignedBoundingBox {
	float minBound[3];	//order x y z
	float maxBound[3];	//order x y z
}aabb_t;


#endif //_VOXEL_H