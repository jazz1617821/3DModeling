#ifndef _VOXEL_H
#define _VOXEL_H

typedef struct Voxel
{
	unsigned char data;
}voxel_t;

typedef struct VoxelData
{
	int resolution[3];
	float voxelsize[3];
	bool isbitcompress;
	voxel_t* rawdata;
}vdata_t;

typedef struct VoxelObject
{

}vobj_t;

typedef struct VoxelModle
{

}vmodel_t;


#endif //_VOXEL_H