#ifndef _VOXEL_H
#define _VOXEL_H

typedef struct Voxel
{
	unsigned char data;

}voxel_t;

typedef struct VoxelData
{
	char name[256];			//體素物件名稱
	int resolution[3];		//此Data解析度
	float voxelsize[3];		//此Data體素大小
	bool isbitcompress;		//
	Voxel * rawdata;
}vdata_t;

typedef struct VoxelObject
{
	//VoxelObject
	char name[256];						//體素物件名稱
	float max_bound[3];					//在VM中的位置
	float min_bound[3];					//
	int number_of_child;				//物件子節點個數
	VoxelObject * parent;
	VoxelObject ** child;
										//橫向優先展開
	//VoxelData
	char voxel_object_fiename[256];		//體素資料檔案名稱 *leaf only
}vobj_t;

typedef struct VoxelModel
{
	char name[256];						//整個模型名稱
	int resolution[3];					//模型最大解析度
	float voxelsize[3];					//體素大小
	int number_of_voxel_data;			//體素資料個數

	char voxel_object_filename[256];	//體素物件檔案名稱
}vmodel_t;


#endif //_VOXEL_H