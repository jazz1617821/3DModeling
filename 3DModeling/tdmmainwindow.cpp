#include "tdmmainwindow.h"
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>

using namespace std;

TDMMainWindow::TDMMainWindow(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::TDMMainWindow)
{
	ui->setupUi(this);

	connect(ui->voxeltreeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(setAttribute(QTreeWidgetItem *, int)));
	connect(ui->datalistWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(setAttribute(QListWidgetItem *)));
	connect(this, SIGNAL(sentVModelPtr(vmodel_t*)), ui->viewWidget, SLOT(getVModelPtr(vmodel_t *)));
	connect(this, SIGNAL(sentVDataPtr(vdata_t*)), ui->editWidget, SLOT(getVDataPtr(vdata_t *)));
}

TDMMainWindow::~TDMMainWindow()
{


}

void TDMMainWindow::makeTestFile(void)
{
	cout << "Make test file." << endl;
	cout << "//--------------------------------test file--------------------------------//" << endl
		 << "//                              ---------                                  //" << endl
		 << "//                              |  vm   |                                  //" << endl
		 << "//                              ---------                                  //" << endl
		 << "//                                  |                                      //" << endl
		 << "//                             ------------                                //" << endl
		 << "//                             | root_obj |                                //" << endl
		 << "//                             ------------                                //" << endl
		 << "//                             /           \\                               //" << endl
		 << "//                            /             \\                              //" << endl
		 << "//                    --------------     --------------                    //" << endl
		 << "//                    | temp_obj_1 |     | temp_obj_2 |                    //" << endl
		 << "//                    --------------     --------------                    //" << endl
		 << "//                   /             |             |                         //" << endl
		 << "//                  /              |             |                         //" << endl
		 << "//        --------------     --------------   ------------                 //" << endl
		 << "//        |temp_obj_1_1|     |temp_obj_1_2|   |dataitem_3|                 //" << endl
		 << "//        --------------     --------------   ------------                 //" << endl
		 << "//             |                   |                                       //" << endl
		 << "//             |                   |                                       //" << endl
		 << "//         ------------      ------------                                  //" << endl
		 << "//         |dataitem_1|      |dataitem_2|                                  //" << endl
		 << "//         ------------      ------------                                  //" << endl
		 << "//-------------------------------test file---------------------------------//" << endl;



	int resolution[3] = { 256,256,256 };
	float voxelsize[3] = { 1.0,0.5,1.0 };
	bool isbitcompress = 1;
	unsigned char* buffer = (unsigned char *)calloc(sizeof(unsigned char), resolution[0] * resolution[1] * resolution[2]);

	for (int i = 0; i < 1000; i++) {
		buffer[(rand() % 256) + (rand() % 256) * 256 + (rand() % 256) * 256 * 256] = 1;
	}

	VoxelData* tempData_1 = new VoxelData;
	tempData_1->name = (char*)malloc(sizeof(char) * 256);
	strcpy(tempData_1->name, "Data_0");
	for (int i = 0; i < 3; i++) {
		tempData_1->resolution[i] = resolution[i];
		tempData_1->voxelSize[i] = voxelsize[i];
	}
	tempData_1->isBitCompress = isbitcompress;

	(Voxel *)tempData_1->rawData = (Voxel *)calloc(resolution[0] * resolution[1] * resolution[2], sizeof(Voxel));

	for (int i = 0; i < resolution[0] * resolution[1] * resolution[2]; i++) {
		tempData_1->rawData[i].data = buffer[i];
	}
	ditem_t* dataitem_1 = new ditem_t;
	dataitem_1->voxlData = new VoxelData;
	dataitem_1->voxlData = tempData_1;

	VoxelData* tempData_2 = new VoxelData;
	tempData_2->name = (char*)malloc(sizeof(char) * 256);
	strcpy(tempData_2->name, "Data_1");
	for (int i = 0; i < 3; i++) {
		tempData_2->resolution[i] = resolution[i];
		tempData_2->voxelSize[i] = voxelsize[i];
	}
	tempData_2->isBitCompress = isbitcompress;

	(Voxel *)tempData_2->rawData = (Voxel *)calloc(resolution[0] * resolution[1] * resolution[2], sizeof(Voxel));

	for (int i = 0; i < resolution[0] * resolution[1] * resolution[2]; i++) {
		tempData_2->rawData[i].data = buffer[i];
	}
	ditem_t* dataitem_2 = new ditem_t;
	dataitem_2->voxlData = new VoxelData;
	dataitem_2->voxlData = tempData_2;

	VoxelData* tempData_3 = new VoxelData;
	tempData_3->name = (char*)malloc(sizeof(char) * 256);
	strcpy(tempData_3->name, "Data_2");
	for (int i = 0; i < 3; i++) {
		tempData_3->resolution[i] = resolution[i];
		tempData_3->voxelSize[i] = voxelsize[i];
	}
	tempData_3->isBitCompress = isbitcompress;

	(Voxel *)tempData_3->rawData = (Voxel *)calloc(resolution[0] * resolution[1] * resolution[2], sizeof(Voxel));

	for (int i = 0; i < resolution[0] * resolution[1] * resolution[2]; i++) {
		tempData_3->rawData[i].data = buffer[i];
	}
	ditem_t* dataitem_3 = new ditem_t;
	dataitem_3->voxlData = new VoxelData;
	dataitem_3->voxlData = tempData_3;


	free(buffer);


	vmodel = new VoxelModel;
	vmodel->name = (char*)malloc(sizeof(char) * 256);
	strcpy(vmodel->name, "Model_0");
	for (int i = 0; i < 3; i++) {
		vmodel->resolution[i] = 4096;
		vmodel->voxelSize[i] = 1;
	}
	vmodel->numberOfVoxelData = 3;

	//Creat root object
	vmodel->rootObj = new vobj_t;
	vmodel->rootObj->name = (char*)malloc(sizeof(char) * 256);
	vmodel->rootObj->bbox = new aabb_t;
	strcpy(vmodel->rootObj->name, "Object_0");
	for (int i = 0; i < 3; i++) {
		vmodel->rootObj->bbox->maxBound[i] = 4096;
		vmodel->rootObj->bbox->minBound[i] = 0;
	}
	vmodel->rootObj->numberOfChild = 2;

	(vobj_t*)vmodel->rootObj->firstChild = (vobj_t*)malloc(sizeof(vobj_t *) * 1);


	//Create 2 object
	vobj_t * tempobj_1 = new vobj_t;
	tempobj_1->name = (char*)malloc(sizeof(char) * 256);
	strcpy(tempobj_1->name, "Object_1");
	tempobj_1->bbox = new aabb_t;
	for (int i = 0; i < 3; i++) {
		tempobj_1->bbox->maxBound[i] = 2048;
		tempobj_1->bbox->minBound[i] = 0;
	}
	tempobj_1->numberOfChild = 2;

	vobj_t * tempobj_2 = new vobj_t;
	tempobj_2->name = (char*)malloc(sizeof(char) * 256);
	strcpy(tempobj_2->name, "Object_2");
	tempobj_2->bbox = new aabb_t;
	for (int i = 0; i < 3; i++) {
		tempobj_2->bbox->maxBound[i] = 4096;
		tempobj_2->bbox->minBound[i] = 2048;
	}
	tempobj_2->numberOfChild = 1;
	tempobj_2->dataItem = dataitem_2;


	//2 Leaf obj
	vobj_t * tempobj_1_1 = new vobj_t;
	tempobj_1_1->name = (char*)malloc(sizeof(char) * 256);
	strcpy(tempobj_1_1->name, "Object_1_1");
	tempobj_1_1->bbox = new aabb_t;
	for (int i = 0; i < 3; i++) {
		tempobj_1_1->bbox->maxBound[i] = 4096;
		tempobj_1_1->bbox->minBound[i] = 0;
	}
	tempobj_1_1->numberOfChild = 1;
	tempobj_1_1->dataItem = dataitem_3;

	vobj_t * tempobj_1_2 = new vobj_t;
	tempobj_1_2->name = (char*)malloc(sizeof(char) * 256);
	strcpy(tempobj_1_2->name, "Object_1_1");
	tempobj_1_2->bbox = new aabb_t;
	for (int i = 0; i < 3; i++) {
		tempobj_1_2->bbox->maxBound[i] = 4096;
		tempobj_1_2->bbox->minBound[i] = 0;
	}
	tempobj_1_2->numberOfChild = 1;
	tempobj_1_2->dataItem = dataitem_3;

	//conect link
	vmodel->rootObj->firstChild = tempobj_1;

	tempobj_1->nextSibling = tempobj_2;
	tempobj_1->parent = vmodel->rootObj;
	tempobj_1->firstChild = tempobj_1_1;

	tempobj_1_1->nextSibling = tempobj_1_2;
	tempobj_1_1->parent = tempobj_1;
	tempobj_1_1->dataItem = dataitem_1;

	tempobj_1_2->prevSibling = tempobj_1_1;
	tempobj_1_2->parent = tempobj_1;
	tempobj_1_2->dataItem = dataitem_2;

	tempobj_2->prevSibling = tempobj_1;
	tempobj_2->parent = vmodel->rootObj;
	tempobj_2->dataItem = dataitem_3;

	cout << "Make test file end." << endl;
	return;
}

//Private Fuction

void TDMMainWindow::addInDataList_VO(vobj_t * vo) 
{
	if (vo->numberOfChild == 1) {
		MyListWidgetItem* itm = new MyListWidgetItem();
		itm->dataItem = vo->dataItem;
		itm->setText(vo->dataItem->voxlData->name);
		ui->datalistWidget->addItem(itm);
		return;
	}
	else {
		vobj_t* voxelObjectPtr = vo->firstChild;
		while (voxelObjectPtr != NULL) {
			addInDataList_VO(voxelObjectPtr);
			voxelObjectPtr = voxelObjectPtr->nextSibling;
		}
	}
}


void TDMMainWindow::addInTreeList_VD(MyTreeWidgetItem * parent, ditem_t* dataItem)
{
	MyTreeWidgetItem * itm = new MyTreeWidgetItem();
	itm->setText(0, (QString)dataItem->voxlData->name);
	itm->dataItem = dataItem;
	parent->addChild(itm);
}

void TDMMainWindow::addInTreeList_VO(MyTreeWidgetItem * parent, vobj_t* vobject)
{
	MyTreeWidgetItem * itm = new MyTreeWidgetItem();
	itm->setText(0, (QString)vobject->name);
	itm->vo = vobject;
	parent->addChild(itm);
	if (vobject->numberOfChild == 1) {
		addInTreeList_VD(itm, vobject->dataItem);
	}
	else {
		vobj_t* voxelObjectPtr = vobject->firstChild;
		while (voxelObjectPtr != NULL) {
			addInTreeList_VO(parent, voxelObjectPtr);
			voxelObjectPtr = voxelObjectPtr->nextSibling;
		}
	}
}

void TDMMainWindow::addInTreeList_VM(vmodel_t* vmodel)
{
	MyTreeWidgetItem * itm = new MyTreeWidgetItem(ui->voxeltreeWidget);
	itm->setText(0,(QString)vmodel->name);
	itm->vm = vmodel;
	addInTreeList_VO(itm, vmodel->rootObj);
}

void TDMMainWindow::openVD(const char* filepath)
{
	int resolution[3];
	float voxelsize[3];
	bool isbitcompress;
	unsigned char * buffer;

	FILE * vdatafile = fopen(filepath, "rb");
	if (vdatafile!=NULL) {
		fread(resolution, 3, sizeof(int), vdatafile);
		fread(voxelsize, 3, sizeof(float), vdatafile);

		fread(&isbitcompress, 1, sizeof(bool), vdatafile);

		(unsigned char *)buffer = (unsigned char *)calloc(resolution[0] * resolution[1] * resolution[2], sizeof(unsigned char));

		fread(buffer, resolution[0] * resolution[1] * resolution[2], sizeof(unsigned char), vdatafile);
	}
	fclose(vdatafile);
	
	//Creat a VoxelData
	VoxelData* temp = new VoxelData;
	temp->name = (char*)malloc(sizeof(char) * 256);
	strcpy(temp->name, "Data_0");
	for (int i = 0; i < 3;i++) {
		temp->resolution[i] = resolution[i];
		temp->voxelSize[i] = voxelsize[i];
	}
	temp->isBitCompress = isbitcompress;

	(Voxel *)temp->rawData = (Voxel *)calloc(resolution[0] * resolution[1] * resolution[2] , sizeof(Voxel));
	
	for (int i = 0; i < resolution[0] * resolution[1] * resolution[2];i++) {
		temp->rawData[i].data = buffer[i];
	}

	free(buffer);


	if(vmodel==NULL) {
		//Creat Model
		vmodel = new VoxelModel;
		vmodel->name = (char*)malloc(sizeof(char) * 256);
		strcpy(vmodel->name, "Model_0");
		for (int i = 0; i < 3; i++) {
			vmodel->resolution[i] = resolution[i];
			vmodel->voxelSize[i] = voxelsize[i];
		}
		vmodel->numberOfVoxelData = 1;
		
		//Creat root object
		vmodel->rootObj = new VoxelObject;
		vmodel->rootObj->name = (char*)malloc(sizeof(char) * 256);
		strcpy(vmodel->rootObj->name, "Object_0");
		for (int i = 0; i < 3; i++) {
			vmodel->rootObj->bbox->maxBound[0] = resolution[0];
			vmodel->rootObj->bbox->minBound[0] = 0;
		}
		vmodel->rootObj->numberOfChild = 1;
		vmodel->rootObj->dataItem->voxlData = temp;

	}
	else {
		//merge vdata into exist vmodel



	}

	addInTreeList_VM(vmodel);

	delete temp;

	return;
}

void TDMMainWindow::openVD(const char * filepath, vdata_t * vd)
{
	char* str = (char*)malloc(sizeof(char)* (strlen(filepath) + strlen(vd->name) + 5));
	strcpy(str, filepath);
	char* i = strrchr(str, '/');
	str[i - str + 1] = '\0';
	strcat(str, vd->name);
	strcat(str, ".vdat");
	cout << str << endl;
	FILE* vdatafile = fopen(str, "rb");

	if (vdatafile != NULL) {
		fread(vd->resolution, sizeof(int), 3, vdatafile);
		cout << vd->resolution[0] << " " << vd->resolution[1] << " " << vd->resolution[2] << endl;
		fread(vd->voxelSize, sizeof(float), 3, vdatafile);
		cout << vd->voxelSize[0] << " " << vd->voxelSize[1] << " " << vd->voxelSize[2] << endl;
		fread(&vd->isBitCompress, sizeof(int), 1, vdatafile);
		cout << vd->isBitCompress << endl;

		(Voxel*)vd->rawData = (Voxel *)calloc(sizeof(Voxel) , vd->resolution[0] * vd->resolution[1] * vd->resolution[2]);

		for (int i = 0; i < vd->resolution[0] * vd->resolution[1] * vd->resolution[2]; i++) {
			fread(&vd->rawData[i].data, sizeof(unsigned char), 1, vdatafile);
		}
		fclose(vdatafile);
	}

	free(str);

	return;
}

void TDMMainWindow::openVO(const char* filepath,FILE * vmodelfile, vobj_t* vo)
{
	(char*)vo->name = (char*)calloc(sizeof(char) , 20);
	fscanf(vmodelfile, "Name:%s\n", vo->name);
	cout << vo->name << endl;
	vo->bbox = new aabb_t;
	fscanf(vmodelfile, "Max bound:%fx%fx%f\n", &vo->bbox->maxBound[0], &vo->bbox->maxBound[1], &vo->bbox->maxBound[2]);
	cout << vo->bbox->maxBound[0]<< " " << vo->bbox->maxBound[1] << " " << vo->bbox->maxBound[2] << endl;
	fscanf(vmodelfile, "Min bound:%fx%fx%f\n", &vo->bbox->minBound[0], &vo->bbox->minBound[1], &vo->bbox->minBound[2]);
	cout << vo->bbox->minBound[0] << " " << vo->bbox->minBound[1] << " " << vo->bbox->minBound[2] << endl;
	fscanf(vmodelfile, "Number of child:%d\n", &vo->numberOfChild);
	cout << vo->numberOfChild << endl;

	if (vo->numberOfChild == 1) {
		vo->dataItem = new ditem_t;
		vo->dataItem->parent = vo;
		vo->dataItem->voxlData = new vdata_t;
		vo->dataItem->voxlData->name = (char*)calloc(sizeof(char) , 20);
		fscanf(vmodelfile, "Voxel data filename:%s\n", vo->dataItem->voxlData->name);
		cout << vo->dataItem->voxlData->name << endl;
		openVD(filepath, vo->dataItem->voxlData);
		return;
	}
	else {
		vo->firstChild = new vobj_t;
		vobj_t* voxelObjectPtr = vo->firstChild;
		for (int i = 0; i < vo->numberOfChild; i++) {
			voxelObjectPtr->parent = vo;
			openVO(filepath, vmodelfile, voxelObjectPtr);
			if (i == vo->numberOfChild - 1) {
				voxelObjectPtr->nextSibling = NULL;
			}
			else {
				voxelObjectPtr->nextSibling = new vobj_t;
				voxelObjectPtr->nextSibling->prevSibling = voxelObjectPtr;
				voxelObjectPtr = voxelObjectPtr->nextSibling;
			}
		}
	}
}

void TDMMainWindow::openVM(const char* filepath)
{
	vmodel = new vmodel_t;

	FILE * vmodelfile = fopen(filepath, "r");
	if (vmodelfile != NULL) {
		char* str = (char*)malloc(sizeof(char) * 10);
		char* str1 = (char*)malloc(sizeof(char) * 10);
		fscanf(vmodelfile, "%s %s\n", str,str1);
		cout << str << " " << str1 << endl;
		vmodel->name = (char*)calloc(sizeof(char), 20);
		fscanf(vmodelfile, "Name:%s\n", vmodel->name);
		cout << vmodel->name << endl;
		fscanf(vmodelfile, "Resolution:%dx%dx%d\n", &vmodel->resolution[0], &vmodel->resolution[1], &vmodel->resolution[2]);
		cout << vmodel->resolution[0] << " " << vmodel->resolution[1] << " " << vmodel->resolution[2] << " " << endl;
		fscanf(vmodelfile, "Voxelsize:%f:%f:%f\n", &vmodel->voxelSize[0], &vmodel->voxelSize[1], &vmodel->voxelSize[2]);
		cout << vmodel->voxelSize[0] << " " << vmodel->voxelSize[1] << " " << vmodel->voxelSize[2] << " " << endl;
		fscanf(vmodelfile, "Number of voxel data:%d\n", &vmodel->numberOfVoxelData);
		cout << vmodel->numberOfVoxelData << endl;

		vmodel->rootObj = new vobj_t;
		openVO(filepath, vmodelfile, vmodel->rootObj);
	}

	fclose(vmodelfile);

	addInTreeList_VM(vmodel);
	addInDataList_VO(vmodel->rootObj);
	setAttribute(vmodel);
	finishLoadVModel();

	return;
}

void TDMMainWindow::writeVO(const char* filepath,FILE* vmodelfile, vobj_t * vo)
{8+
	//Write this object
	fprintf(vmodelfile, "Name:%s\n", vo->name);
	fprintf(vmodelfile, "Max bound:%fx%fx%f\n", vo->bbox->maxBound[0], vo->bbox->maxBound[1], vo->bbox->maxBound[2]);
	fprintf(vmodelfile, "Min bound:%fx%fx%f\n", vo->bbox->minBound[0], vo->bbox->minBound[1], vo->bbox->minBound[2]);
	fprintf(vmodelfile, "Number of child:%d\n", vo->numberOfChild);

	if (vo->numberOfChild == 1) {
		fprintf(vmodelfile,"Voxel data filename:%s\n",vo->dataItem->voxlData->name);
		writeVD(filepath,vo->dataItem->voxlData);
		return;
	}
	else {
		vobj_t* voxelObjPtr = vo->firstChild;
		while (voxelObjPtr != NULL) {
			writeVO(filepath, vmodelfile, voxelObjPtr);
			voxelObjPtr = voxelObjPtr->nextSibling;
		}
	}
}

void TDMMainWindow::writeVD(const char* filepath, vdata_t * vd)
{
	char* str = (char*)malloc(sizeof(char)* (strlen(filepath) + strlen(vd->name) + 5));
	strcpy(str, filepath);
	char* i = strrchr(str, '/');
	str[i - str + 1] = '\0';
	strcat(str, vd->name);
	strcat(str, ".vdat");
	cout << str << endl;
	FILE* vdatafile = fopen(str, "wb");
	
	if(vdatafile) {
		/*
		fprintf(vdatafile, "Name:%s\n", vd->name);
		fprintf(vdatafile, "Resolution:%dx%dx%d\n", vd->resolution[0], vd->resolution[1], vd->resolution[2]);
		fprintf(vdatafile, "Voxelsize:%.2fx%.2fx%.2f\n", vd->voxelsize[0], vd->voxelsize[1], vd->voxelsize[2]);
		fprintf(vdatafile, "Bit:%d\n", vd->isbitcompress);
		for (int i = 0; i < vd->resolution[0] * vd->resolution[1] * vd->resolution[2]; i++) {
			fprintf(vdatafile, "%c", vd->rawdata[i].data);
		}*/

		fwrite(vd->resolution, sizeof(int), 3, vdatafile);
		fwrite(vd->voxelSize, sizeof(float), 3, vdatafile);
		fwrite(&vd->isBitCompress, sizeof(int), 1, vdatafile);
		for (int i = 0; i < vd->resolution[0] * vd->resolution[1] * vd->resolution[2]; i++) {
			fwrite(&vd->rawData[i].data, sizeof(unsigned char), 1, vdatafile);
		}
		cout << "Finish write " << vd->name << endl;
		fclose(vdatafile);
	}

	free(str);

	return;
}

void TDMMainWindow::setAttribute(vdata_t* vd) 
{
	ui->attributetableWidget->clear();
	ui->attributetableWidget->setRowCount(4);
	ui->attributetableWidget->setColumnCount(4);

	QTableWidgetItem* namelabel = new QTableWidgetItem("Name:");
	QTableWidgetItem* name = new QTableWidgetItem(QString(vd->name));
	ui->attributetableWidget->setItem(0, 0, namelabel);
	ui->attributetableWidget->setItem(0, 1, name);
	QTableWidgetItem* resolutionlabel = new QTableWidgetItem("Resolution:");
	QTableWidgetItem* resolution_x = new QTableWidgetItem(QString::number(vd->resolution[0]));
	QTableWidgetItem* resolution_y = new QTableWidgetItem(QString::number(vd->resolution[1]));
	QTableWidgetItem* resolution_z = new QTableWidgetItem(QString::number(vd->resolution[2]));
	ui->attributetableWidget->setItem(1, 0, resolutionlabel);
	ui->attributetableWidget->setItem(1, 1, resolution_x);
	ui->attributetableWidget->setItem(1, 2, resolution_y);
	ui->attributetableWidget->setItem(1, 3, resolution_z);
	QTableWidgetItem* voxelsizelabel = new QTableWidgetItem("Voxelsize:");
	QTableWidgetItem* voxelsize_x = new QTableWidgetItem(QString::number(vd->voxelSize[0]));
	QTableWidgetItem* voxelsize_y = new QTableWidgetItem(QString::number(vd->voxelSize[1]));
	QTableWidgetItem* voxelsize_z = new QTableWidgetItem(QString::number(vd->voxelSize[2]));
	ui->attributetableWidget->setItem(2, 0, voxelsizelabel);
	ui->attributetableWidget->setItem(2, 1, voxelsize_x);
	ui->attributetableWidget->setItem(2, 2, voxelsize_y);
	ui->attributetableWidget->setItem(2, 3, voxelsize_z);
	QTableWidgetItem* bitcompresslabel = new QTableWidgetItem("Bit compressed:");
	QTableWidgetItem* bitcompress = new QTableWidgetItem(QString::number(vd->isBitCompress));
	ui->attributetableWidget->setItem(3, 0, bitcompresslabel);
	ui->attributetableWidget->setItem(3, 1, bitcompress);

	return;
}

void TDMMainWindow::setAttribute(vobj_t* vo) 
{
	ui->attributetableWidget->clear();
	if (vo->numberOfChild == 1) {
		ui->attributetableWidget->setRowCount(5);
	}
	else {
		ui->attributetableWidget->setRowCount(4 + vo->numberOfChild);
	}
	ui->attributetableWidget->setColumnCount(4);

	QTableWidgetItem* namelabel = new QTableWidgetItem("Name:");
	QTableWidgetItem* name = new QTableWidgetItem(QString(vo->name));
	ui->attributetableWidget->setItem(0, 0, namelabel);
	ui->attributetableWidget->setItem(0, 1, name);
	QTableWidgetItem* max_boundlabel = new QTableWidgetItem("Max bound:");
	QTableWidgetItem* max_bound_x = new QTableWidgetItem(QString::number(vo->bbox->maxBound[0]));
	QTableWidgetItem* max_bound_y = new QTableWidgetItem(QString::number(vo->bbox->maxBound[1]));
	QTableWidgetItem* max_bound_z = new QTableWidgetItem(QString::number(vo->bbox->maxBound[2]));
	ui->attributetableWidget->setItem(1, 0, max_boundlabel);
	ui->attributetableWidget->setItem(1, 1, max_bound_x);
	ui->attributetableWidget->setItem(1, 2, max_bound_y);
	ui->attributetableWidget->setItem(1, 3, max_bound_z);
	QTableWidgetItem* min_boundlabel = new QTableWidgetItem("Min bound:");
	QTableWidgetItem* min_bound_x = new QTableWidgetItem(QString::number(vo->bbox->minBound[0]));
	QTableWidgetItem* min_bound_y = new QTableWidgetItem(QString::number(vo->bbox->minBound[1]));
	QTableWidgetItem* min_bound_z = new QTableWidgetItem(QString::number(vo->bbox->minBound[2]));
	ui->attributetableWidget->setItem(2, 0, min_boundlabel);
	ui->attributetableWidget->setItem(2, 1, min_bound_x);
	ui->attributetableWidget->setItem(2, 2, min_bound_y);
	ui->attributetableWidget->setItem(2, 3, min_bound_z);
	QTableWidgetItem* number_of_childlabel = new QTableWidgetItem("Number of child:");
	QTableWidgetItem* number_of_child = new QTableWidgetItem(QString::number(vo->numberOfChild));
	ui->attributetableWidget->setItem(3, 0, number_of_childlabel);
	ui->attributetableWidget->setItem(3, 1, number_of_child);
	if (vo->numberOfChild == 1) {
		QTableWidgetItem* voxeldatanamelabel = new QTableWidgetItem("Voxel Data:");
		QTableWidgetItem* voxeldataname = new QTableWidgetItem(vo->dataItem->voxlData->name);
		ui->attributetableWidget->setItem(4, 0, voxeldatanamelabel);
		ui->attributetableWidget->setItem(4, 1, voxeldataname);
	}
	else {
		QTableWidgetItem* childnamelabel = new QTableWidgetItem("Child Object:");
		ui->attributetableWidget->setItem(4, 0, childnamelabel);
		vobj_t* voxelObjectPtr = vo->firstChild;
		int count = 0;
		while (voxelObjectPtr != NULL) {
			QTableWidgetItem* childname = new QTableWidgetItem(voxelObjectPtr->name);
			ui->attributetableWidget->setItem(count + 4, 1, childname);

			voxelObjectPtr = voxelObjectPtr->nextSibling;
			count++;
		}
	}
	return;
}

void TDMMainWindow::setAttribute(vmodel_t* vm) 
{
	ui->attributetableWidget->clear();
	ui->attributetableWidget->setRowCount(4);
	ui->attributetableWidget->setColumnCount(4);

	QTableWidgetItem* namelabel = new QTableWidgetItem("Name:");
	QTableWidgetItem* name = new QTableWidgetItem(QString(vm->name));
	ui->attributetableWidget->setItem(0, 0, namelabel);
	ui->attributetableWidget->setItem(0, 1, name);
	QTableWidgetItem* resolutionlabel = new QTableWidgetItem("Resolution:");
	QTableWidgetItem* resolution_x = new QTableWidgetItem(QString::number(vm->resolution[0]));
	QTableWidgetItem* resolution_y = new QTableWidgetItem(QString::number(vm->resolution[1]));
	QTableWidgetItem* resolution_z = new QTableWidgetItem(QString::number(vm->resolution[2]));
	ui->attributetableWidget->setItem(1, 0, resolutionlabel);
	ui->attributetableWidget->setItem(1, 1, resolution_x);
	ui->attributetableWidget->setItem(1, 2, resolution_y);
	ui->attributetableWidget->setItem(1, 3, resolution_z);
	QTableWidgetItem* voxelsizelabel = new QTableWidgetItem("Voxelsize:");
	QTableWidgetItem* voxelsize_x = new QTableWidgetItem(QString::number(vm->voxelSize[0]));
	QTableWidgetItem* voxelsize_y = new QTableWidgetItem(QString::number(vm->voxelSize[1]));
	QTableWidgetItem* voxelsize_z = new QTableWidgetItem(QString::number(vm->voxelSize[2]));
	ui->attributetableWidget->setItem(2, 0, voxelsizelabel);
	ui->attributetableWidget->setItem(2, 1, voxelsize_x);
	ui->attributetableWidget->setItem(2, 2, voxelsize_y);
	ui->attributetableWidget->setItem(2, 3, voxelsize_z);
	QTableWidgetItem* number_of_voxel_datalabel = new QTableWidgetItem("Number of voxel data:");
	QTableWidgetItem* number_of_voxel_data = new QTableWidgetItem(QString::number(vm->numberOfVoxelData));
	ui->attributetableWidget->setItem(3, 0, number_of_voxel_datalabel);
	ui->attributetableWidget->setItem(3, 1, number_of_voxel_data);

	return;
}

//Private Slots
void TDMMainWindow::on_actionOpen_triggered()
{
	QString filename = QFileDialog::getOpenFileName(
		this,
		tr("Open Voxel Model"),
		"",
		tr("Voxel Files (*.vdat *.vm)")
		);

	QByteArray ba = filename.toLatin1();
	const char *fileLocationStr = ba.data();
	printf("File location:%s\n", fileLocationStr);

	const char *str = ba.data();
	const char *fe;

	fe = strrchr(str, '.');
	if (!filename.isEmpty()) {
		if (!strcmp(fe, ".vdat")) {
			openVD(str);
		}
		if (!strcmp(fe, ".vm")) {
			openVM(str);
		}
	}
}

void TDMMainWindow::on_actionSave_triggered()
{
	if (vmodel == NULL) {
		makeTestFile();
	}

	QString filename;
	const char *str;
	const char *fe;

	if (vmodel != NULL) {
		filename = QFileDialog::getSaveFileName(
			this,
			tr("Save File"),
			"untitled",
			tr("Model Files (*.vm);; Data Files (*.vdat)"));
		QByteArray ba = filename.toLatin1();
		str = ba.data();
		fe = strrchr(str, '.');

		if (!filename.isEmpty()) {
			if (!strcmp(fe, ".vm")) {
				//export vmodel

				FILE* vmodelfile = fopen(str, "w");

				//Write vmodel 
				fprintf(vmodelfile, "Voxel Model:\n");
				fprintf(vmodelfile, "Name:%s\n", vmodel->name);
				fprintf(vmodelfile, "Resolution:%dx%dx%d\n", vmodel->resolution[0], vmodel->resolution[1], vmodel->resolution[2]);
				fprintf(vmodelfile, "Voxelsize:%.2f:%.2f:%.2f\n", vmodel->voxelSize[0], vmodel->voxelSize[1], vmodel->voxelSize[2]);
				fprintf(vmodelfile, "Number of voxel data:%d\n", vmodel->numberOfVoxelData);

				//DFS Write Voxel Object
				writeVO(str, vmodelfile, vmodel->rootObj);

				//
				fclose(vmodelfile);
				return;
			}
			else if (!strcmp(fe, ".vdat")) {
				//export vdata


			}
		}
		else {
			QMessageBox::information(this, tr("Warning"), "Failed to save the file.");
		}
	}
	else {
		QMessageBox::information(this, tr("Warning"), "There is no data to export.");
	}
}

void TDMMainWindow::setAttribute(QTreeWidgetItem * itm, int i) {
	ui->view_tabWidget->setCurrentWidget(ui->viewWidgetLayout);
	if (((MyTreeWidgetItem *)itm)->dataItem != NULL) {
		//cout << "I am a Voxel Data." << endl;
		setAttribute(((MyTreeWidgetItem *)itm)->dataItem->voxlData);
	}
	else if (((MyTreeWidgetItem *)itm)->vo != NULL) {
		//cout << "I am a Voxel Object." << endl;
		setAttribute(((MyTreeWidgetItem *)itm)->vo);
	}
	else if (((MyTreeWidgetItem *)itm)->vm != NULL) {
		//cout << "I am a Voxel Model." << endl;
		setAttribute(((MyTreeWidgetItem *)itm)->vm);
	}
	return;
}

void TDMMainWindow::setAttribute(QListWidgetItem * itm) {
	setAttribute(((MyListWidgetItem *)itm)->dataItem->voxlData);
	ui->view_tabWidget->setCurrentWidget(ui->editWidgetLayout);
	emit sentVDataPtr(((MyListWidgetItem *)itm)->dataItem->voxlData);
	return;
}

//Signal trigger function
void TDMMainWindow::finishLoadVModel() {
	emit sentVModelPtr(vmodel);
	return;
}
