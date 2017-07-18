#ifndef _VOXEL_H
#define _VOXEL_H

typedef struct Voxel
{
	unsigned char data;

}voxel_t;

typedef struct VoxelData
{
	char name[256];			//�������W��
	int resolution[3];		//��Data�ѪR��
	float voxelsize[3];		//��Data����j�p
	bool isbitcompress;		//
	Voxel * rawdata;
}vdata_t;

typedef struct VoxelObject
{
	//VoxelObject
	char name[256];						//�������W��
	float max_bound[3];					//�bVM������m
	float min_bound[3];					//
	int number_of_child;				//����l�`�I�Ӽ�
	VoxelObject * parent;
	VoxelObject ** child;
										//��V�u���i�}
	//VoxelData
	char voxel_object_fiename[256];		//�������ɮצW�� *leaf only
}vobj_t;

typedef struct VoxelModel
{
	char name[256];						//��Ӽҫ��W��
	int resolution[3];					//�ҫ��̤j�ѪR��
	float voxelsize[3];					//����j�p
	int number_of_voxel_data;			//�����ƭӼ�

	char voxel_object_filename[256];	//��������ɮצW��
}vmodel_t;


#endif //_VOXEL_H