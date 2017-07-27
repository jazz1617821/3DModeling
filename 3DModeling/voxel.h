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

typedef struct Voxel
{
	unsigned char data;
}voxel_t;

typedef struct VoxelData
{
	char* name;				//�������W��
	int resolution[3];		//��Data�ѪR��
	float voxelsize[3];		//��Data����j�p
	bool isbitcompress;		//
	Voxel * rawdata;
}vdata_t;

typedef struct VoxelObject
{
	//VoxelObject
	char* name;						//�������W��
	float max_bound[3];				//�bVM������m
	float min_bound[3];				//
	int number_of_child;			//����l�`�I�Ӽ�
	VoxelObject * parent;			//father pointer
	VoxelObject ** child;			//Child array;
									//��V�u���i�}
	//VoxelData
	VoxelData* vd;					//leaft only
}vobj_t;

typedef struct VoxelModel
{
	char* name;						//��Ӽҫ��W��
	int resolution[3];				//�ҫ��̤j�ѪR��
	float voxelsize[3];				//����j�p
	int number_of_voxel_data;		//�����ƭӼ�

	VoxelObject* root_vobj;			//

}vmodel_t;


#endif //_VOXEL_H