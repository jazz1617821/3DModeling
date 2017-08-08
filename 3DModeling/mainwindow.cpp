#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->voxeltreeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(setAttribute(QTreeWidgetItem *, int)));
	connect(ui->datalistWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(setAttribute(QListWidgetItem *)));
	connect(this, SIGNAL(sentVModelPtr(vmodel_t*)), ui->viewWidget, SLOT(getVModelPtr(vmodel_t *)));
	connect(this, SIGNAL(sentVDataPtr(vdata_t*)), ui->editWidget, SLOT(getVDataPtr(vdata_t *)));
}

MainWindow::~MainWindow()
{


}

//Private Fuction
void MainWindow::addInDataList_VO(vobj_t * vo) 
{
	if (vo->numberOfChild == 1) {
		MyListWidgetItem* itm = new MyListWidgetItem();
		itm->vd = vo->vd;
		itm->setText(vo->vd->name);
		ui->datalistWidget->addItem(itm);
		return;
	}
	else {
		for (int i = 0; i < vo->number_of_child; i++) {
			addInDataList_VO(vo->child[i]);
		}
	}
}

void MainWindow::addInTreeList_VD(MyTreeWidgetItem * parent, vdata_t* vdata)
{
	MyTreeWidgetItem * itm = new MyTreeWidgetItem();
	itm->setText(0, (QString)vdata->name);
	itm->vd = vdata;
	parent->addChild(itm);
}

void MainWindow::addInTreeList_VO(MyTreeWidgetItem * parent, vobj_t* vobject)
{
	MyTreeWidgetItem * itm = new MyTreeWidgetItem();
	itm->setText(0, (QString)vobject->name);
	itm->vo = vobject;
	parent->addChild(itm);
	if (vobject->number_of_child > 1) {
		for (int i = 0; i < vobject->number_of_child; i++) {
			addInTreeList_VO(parent, vobject->child[i]);
		}
	}
	else {
		addInTreeList_VD(itm, vobject->vd);
	}
}

void MainWindow::addInTreeList_VM(vmodel_t* vmodel)
{
	MyTreeWidgetItem * itm = new MyTreeWidgetItem(ui->voxeltreeWidget);
	itm->setText(0,(QString)vmodel->name);
	itm->vm = vmodel;
	addInTreeList_VO(itm, vmodel->root_vobj);
}

void MainWindow::openVD(const char* filepath)
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
		temp->voxelsize[i] = voxelsize[i];
	}
	temp->isbitcompress = isbitcompress;

	(Voxel *)temp->rawdata = (Voxel *)calloc(resolution[0] * resolution[1] * resolution[2] , sizeof(Voxel));
	
	for (int i = 0; i < resolution[0] * resolution[1] * resolution[2];i++) {
		temp->rawdata[i].data = buffer[i];
	}

	free(buffer);


	if(vmodel==NULL) {
		//Creat Model
		vmodel = new VoxelModel;
		vmodel->name = (char*)malloc(sizeof(char) * 256);
		strcpy(vmodel->name, "Model_0");
		for (int i = 0; i < 3; i++) {
			vmodel->resolution[i] = resolution[i];
			vmodel->voxelsize[i] = voxelsize[i];
		}
		vmodel->number_of_voxel_data = 1;
		
		//Creat root object
		vmodel->root_vobj = new VoxelObject;
		vmodel->root_vobj->name = (char*)malloc(sizeof(char) * 256);
		strcpy(vmodel->root_vobj->name, "Object_0");
		for (int i = 0; i < 3; i++) {
			vmodel->root_vobj->max_bound[0] = resolution[0];
			vmodel->root_vobj->min_bound[0] = 0;
		}
		vmodel->root_vobj->number_of_child = 1;
		vmodel->root_vobj->vd = temp;

	}
	else {
		//merge vdata into exist vmodel



	}

	addInTreeList_VM(vmodel);

	delete temp;

	return;
}

void MainWindow::openVD(const char * filepath, vdata_t * vd)
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
		fread(vd->voxelsize, sizeof(float), 3, vdatafile);
		cout << vd->voxelsize[0] << " " << vd->voxelsize[1] << " " << vd->voxelsize[2] << endl;
		fread(&vd->isbitcompress, sizeof(int), 1, vdatafile);
		cout << vd->isbitcompress << endl;

		(Voxel*)vd->rawdata = (Voxel *)calloc(sizeof(Voxel) , vd->resolution[0] * vd->resolution[1] * vd->resolution[2]);

		for (int i = 0; i < vd->resolution[0] * vd->resolution[1] * vd->resolution[2]; i++) {
			fread(&vd->rawdata[i].data, sizeof(unsigned char), 1, vdatafile);
		}
		fclose(vdatafile);
	}

	free(str);

	return;
}

void MainWindow::openVO(const char* filepath,FILE * vmodelfile, vobj_t* vo)
{
	(char*)vo->name = (char*)calloc(sizeof(char) , 20);
	fscanf(vmodelfile, "Name:%s\n", vo->name);
	cout << vo->name << endl;
	fscanf(vmodelfile, "Max bound:%fx%fx%f\n", &vo->max_bound[0], &vo->max_bound[1], &vo->max_bound[2]);
	cout << vo->max_bound[0]<< " " << vo->max_bound[1] << " " << vo->max_bound[2] << endl;
	fscanf(vmodelfile, "Min bound:%fx%fx%f\n", &vo->min_bound[0], &vo->min_bound[1], &vo->min_bound[2]);
	cout << vo->min_bound[0] << " " << vo->min_bound[1] << " " << vo->min_bound[2] << endl;
	fscanf(vmodelfile, "Number of child:%d\n", &vo->number_of_child);
	cout << vo->number_of_child << endl;

	if (vo->number_of_child == 1) {
		vo->vd = new VoxelData;
		vo->vd->name = (char*)calloc(sizeof(char) , 20);
		fscanf(vmodelfile, "Voxel data filename:%s\n", vo->vd->name);
		cout << vo->vd->name << endl;
		openVD(filepath, vo->vd);
		return;
	}
	else {
		(VoxelObject**)vo->child = (VoxelObject**)malloc(sizeof(VoxelObject*)*vo->number_of_child);
		for (int i = 0; i < vo->number_of_child; i++) {
			vo->child[i] = new VoxelObject;
			vo->child[i]->parent = new VoxelObject;
			vo->child[i]->parent = vo;
			openVO(filepath, vmodelfile, vo->child[i]);
		}
	}
}

void MainWindow::openVM(const char* filepath)
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
		fscanf(vmodelfile, "Voxelsize:%f:%f:%f\n", &vmodel->voxelsize[0], &vmodel->voxelsize[1], &vmodel->voxelsize[2]);
		cout << vmodel->voxelsize[0] << " " << vmodel->voxelsize[1] << " " << vmodel->voxelsize[2] << " " << endl;
		fscanf(vmodelfile, "Number of voxel data:%d\n", &vmodel->number_of_voxel_data);
		cout << vmodel->number_of_voxel_data << endl;

		vmodel->root_vobj = new vobj_t;
		openVO(filepath, vmodelfile, vmodel->root_vobj);
	}

	fclose(vmodelfile);

	addInTreeList_VM(vmodel);
	addInDataList_VO(vmodel->root_vobj);
	setAttribute(vmodel);
	finishLoadVModel();

	return;
}

void MainWindow::writeVO(const char* filepath,FILE* vmodelfile,VoxelObject * vo)
{
	//Write this object
	fprintf(vmodelfile, "Name:%s\n", vo->name);
	fprintf(vmodelfile, "Max bound:%fx%fx%f\n", vo->max_bound[0], vo->max_bound[1], vo->max_bound[2]);
	fprintf(vmodelfile, "Min bound:%fx%fx%f\n", vo->min_bound[0], vo->min_bound[1], vo->min_bound[2]);
	fprintf(vmodelfile, "Number of child:%d\n", vo->number_of_child);

	if (vo->number_of_child == 1) {
		fprintf(vmodelfile,"Voxel data filename:%s\n",vo->vd->name);
		writeVD(filepath,vo->vd);
		return;
	}
	else {
		for (int i = 0; i < vo->number_of_child; i++) {
			writeVO(filepath, vmodelfile, vo->child[i]);
		}
	}
}

void MainWindow::writeVD(const char* filepath, vdata_t * vd)
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
		fwrite(vd->voxelsize, sizeof(float), 3, vdatafile);
		fwrite(&vd->isbitcompress, sizeof(int), 1, vdatafile);
		for (int i = 0; i < vd->resolution[0] * vd->resolution[1] * vd->resolution[2]; i++) {
			fwrite(&vd->rawdata[i].data, sizeof(unsigned char), 1, vdatafile);
		}
		cout << "Finish write " << vd->name << endl;
		fclose(vdatafile);
	}

	free(str);

	return;
}

void MainWindow::setAttribute(vdata_t* vd) 
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
	QTableWidgetItem* voxelsize_x = new QTableWidgetItem(QString::number(vd->voxelsize[0]));
	QTableWidgetItem* voxelsize_y = new QTableWidgetItem(QString::number(vd->voxelsize[1]));
	QTableWidgetItem* voxelsize_z = new QTableWidgetItem(QString::number(vd->voxelsize[2]));
	ui->attributetableWidget->setItem(2, 0, voxelsizelabel);
	ui->attributetableWidget->setItem(2, 1, voxelsize_x);
	ui->attributetableWidget->setItem(2, 2, voxelsize_y);
	ui->attributetableWidget->setItem(2, 3, voxelsize_z);
	QTableWidgetItem* bitcompresslabel = new QTableWidgetItem("Bit compressed:");
	QTableWidgetItem* bitcompress = new QTableWidgetItem(QString::number(vd->isbitcompress));
	ui->attributetableWidget->setItem(3, 0, bitcompresslabel);
	ui->attributetableWidget->setItem(3, 1, bitcompress);

	return;
}

void MainWindow::setAttribute(vobj_t* vo) 
{
	ui->attributetableWidget->clear();
	if (vo->number_of_child == 1) {
		ui->attributetableWidget->setRowCount(5);
	}
	else {
		ui->attributetableWidget->setRowCount(4 + vo->number_of_child);
	}
	ui->attributetableWidget->setColumnCount(4);

	QTableWidgetItem* namelabel = new QTableWidgetItem("Name:");
	QTableWidgetItem* name = new QTableWidgetItem(QString(vo->name));
	ui->attributetableWidget->setItem(0, 0, namelabel);
	ui->attributetableWidget->setItem(0, 1, name);
	QTableWidgetItem* max_boundlabel = new QTableWidgetItem("Max bound:");
	QTableWidgetItem* max_bound_x = new QTableWidgetItem(QString::number(vo->max_bound[0]));
	QTableWidgetItem* max_bound_y = new QTableWidgetItem(QString::number(vo->max_bound[1]));
	QTableWidgetItem* max_bound_z = new QTableWidgetItem(QString::number(vo->max_bound[2]));
	ui->attributetableWidget->setItem(1, 0, max_boundlabel);
	ui->attributetableWidget->setItem(1, 1, max_bound_x);
	ui->attributetableWidget->setItem(1, 2, max_bound_y);
	ui->attributetableWidget->setItem(1, 3, max_bound_z);
	QTableWidgetItem* min_boundlabel = new QTableWidgetItem("Min bound:");
	QTableWidgetItem* min_bound_x = new QTableWidgetItem(QString::number(vo->min_bound[0]));
	QTableWidgetItem* min_bound_y = new QTableWidgetItem(QString::number(vo->min_bound[1]));
	QTableWidgetItem* min_bound_z = new QTableWidgetItem(QString::number(vo->min_bound[2]));
	ui->attributetableWidget->setItem(2, 0, min_boundlabel);
	ui->attributetableWidget->setItem(2, 1, min_bound_x);
	ui->attributetableWidget->setItem(2, 2, min_bound_y);
	ui->attributetableWidget->setItem(2, 3, min_bound_z);
	QTableWidgetItem* number_of_childlabel = new QTableWidgetItem("Number of child:");
	QTableWidgetItem* number_of_child = new QTableWidgetItem(QString::number(vo->number_of_child));
	ui->attributetableWidget->setItem(3, 0, number_of_childlabel);
	ui->attributetableWidget->setItem(3, 1, number_of_child);
	if (vo->number_of_child == 1) {
		QTableWidgetItem* voxeldatanamelabel = new QTableWidgetItem("Voxel Data:");
		QTableWidgetItem* voxeldataname = new QTableWidgetItem(vo->vd->name);
		ui->attributetableWidget->setItem(4, 0, voxeldatanamelabel);
		ui->attributetableWidget->setItem(4, 1, voxeldataname);
	}
	else {
		QTableWidgetItem* childnamelabel = new QTableWidgetItem("Child Object:");
		ui->attributetableWidget->setItem(4, 0, childnamelabel);
		for (int i = 0; i < vo->number_of_child; i++) {
			QTableWidgetItem* childname = new QTableWidgetItem(vo->child[i]->name);
			ui->attributetableWidget->setItem(i + 4, 1, childname);
		}
	}
	return;
}

void MainWindow::setAttribute(vmodel_t* vm) 
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
	QTableWidgetItem* voxelsize_x = new QTableWidgetItem(QString::number(vm->voxelsize[0]));
	QTableWidgetItem* voxelsize_y = new QTableWidgetItem(QString::number(vm->voxelsize[1]));
	QTableWidgetItem* voxelsize_z = new QTableWidgetItem(QString::number(vm->voxelsize[2]));
	ui->attributetableWidget->setItem(2, 0, voxelsizelabel);
	ui->attributetableWidget->setItem(2, 1, voxelsize_x);
	ui->attributetableWidget->setItem(2, 2, voxelsize_y);
	ui->attributetableWidget->setItem(2, 3, voxelsize_z);
	QTableWidgetItem* number_of_voxel_datalabel = new QTableWidgetItem("Number of voxel data:");
	QTableWidgetItem* number_of_voxel_data = new QTableWidgetItem(QString::number(vm->number_of_voxel_data));
	ui->attributetableWidget->setItem(3, 0, number_of_voxel_datalabel);
	ui->attributetableWidget->setItem(3, 1, number_of_voxel_data);

	return;
}

//Private Slots
void MainWindow::on_actionOpen_triggered()
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

void MainWindow::on_actionSave_triggered()
{
	
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
	vmodel->rootObj = new VoxelObject;
	vmodel->rootObj->name = (char*)malloc(sizeof(char) * 256);
	strcpy(vmodel->rootObj->name, "Object_0");
	for (int i = 0; i < 3; i++) {
		vmodel->rootObj->bbox->maxBound[i] = 4096;
		vmodel->rootObj->bbox->minBound[i] = 0;
	}
	vmodel->rootObj->numberOfChild = 3;

	(VoxelObject*)vmodel->rootObj->firstChild = (VoxelObject*)malloc(sizeof(VoxelObject *) * 1);

	VoxelObject * tempobj_1 = new VoxelObject;
	tempobj_1->name = (char*)malloc(sizeof(char) * 256);
	strcpy(tempobj_1->name, "Object_1");
	for (int i = 0; i < 3; i++) {
		tempobj_1->bbox->maxBound[i] = 2048;
		tempobj_1->bbox->minBound[i] = 0;
	}
	tempobj_1->numberOfChild = 1;
	tempobj_1->dataItem = tempData_1;

	VoxelObject * tempobj_2 = new VoxelObject;
	tempobj_2->name = (char*)malloc(sizeof(char) * 256);
	strcpy(tempobj_2->name, "Object_2");
	for (int i = 0; i < 3; i++) {
		tempobj_2->max_bound[i] = 4096;
		tempobj_2->min_bound[i] = 2048;
	}
	tempobj_2->number_of_child = 1;
	tempobj_2->vd = tempData_2;

	VoxelObject * tempobj_3 = new VoxelObject;
	tempobj_3->name = (char*)malloc(sizeof(char) * 256);
	strcpy(tempobj_3->name, "Object_3");
	for (int i = 0; i < 3; i++) {
		tempobj_3->max_bound[i] = 4096;
		tempobj_3->min_bound[i] = 0;
	}
	tempobj_3->number_of_child = 1;
	tempobj_3->vd = tempData_3;

	vmodel->root_vobj->child[0] = tempobj_1;
	vmodel->root_vobj->child[1] = tempobj_2;
	vmodel->root_vobj->child[2] = tempobj_3;
	
	

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
				fprintf(vmodelfile, "Voxelsize:%.2f:%.2f:%.2f\n", vmodel->voxelsize[0], vmodel->voxelsize[1], vmodel->voxelsize[2]);
				fprintf(vmodelfile, "Number of voxel data:%d\n", vmodel->number_of_voxel_data);



				//DFS Write Voxel Object
				writeVO(str, vmodelfile, vmodel->root_vobj);

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

void MainWindow::setAttribute(QTreeWidgetItem * itm, int i) {
	ui->view_tabWidget->setCurrentWidget(ui->viewWidgetLayout);
	if (((MyTreeWidgetItem *)itm)->vd != NULL) {
		//cout << "I am a Voxel Data." << endl;
		setAttribute(((MyTreeWidgetItem *)itm)->vd);
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

void MainWindow::setAttribute(QListWidgetItem * itm) {
	setAttribute(((MyListWidgetItem *)itm)->vd);
	ui->view_tabWidget->setCurrentWidget(ui->editWidgetLayout);
	emit sentVDataPtr(((MyListWidgetItem *)itm)->vd);
	return;
}

//Signal trigger function
void MainWindow::finishLoadVModel() {
	emit sentVModelPtr(vmodel);
	return;
}
