#ifndef _VOXEL_H
#define _VOXEL_H

typedef struct Voxel
{
	unsigned char data;

}voxel_t;

typedef struct VoxelData
{
	char* name;			//�������W��
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

	char* root_object_filename;		//
	VoxelObject* root_vobj;			//

}vmodel_t;


#endif //_VOXEL_H