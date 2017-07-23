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

	


}

MainWindow::~MainWindow()
{

}

void MainWindow::addInTreeList_VD(MyTreeWidgetItem * parent, VoxelData* vdata)
{
	MyTreeWidgetItem * itm = new MyTreeWidgetItem();
	itm->setText(0, (QString)vdata->name);
	itm->vd = vdata;
	parent->addChild(itm);
}


void MainWindow::addInTreeList_VO(MyTreeWidgetItem * parent, VoxelObject* vobject)
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

void MainWindow::addInTreeList_VM(VoxelModel* vmodel)
{
	MyTreeWidgetItem * itm = new MyTreeWidgetItem(ui->voxeltreeWidget);
	itm->setText(0,(QString)vmodel->name);
	itm->vm = vmodel;
	addInTreeList_VO(itm, vmodel->root_vobj);
}

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


void MainWindow::on_actionExport_triggered()
{
	int resolution[3] = { 256,256,256 };
	float voxelsize[3] = { 1.0,0.5,1.0 };
	bool isbitcompress = 1;
	unsigned char* buffer = (unsigned char *)calloc(sizeof(unsigned char), resolution[0] * resolution[1] * resolution[2]);

	for (int i = 0; i < resolution[0] * resolution[1] * resolution[2]; i++) {
		buffer[i] = rand() % 256;
	}

	VoxelData* temp = new VoxelData;
	temp->name = (char*)malloc(sizeof(char) * 256);
	strcpy(temp->name, "Data_0");
	for (int i = 0; i < 3; i++) {
		temp->resolution[i] = resolution[i];
		temp->voxelsize[i] = voxelsize[i];
	}
	temp->isbitcompress = isbitcompress;

	(Voxel *)temp->rawdata = (Voxel *)calloc(resolution[0] * resolution[1] * resolution[2], sizeof(Voxel));

	for (int i = 0; i < resolution[0] * resolution[1] * resolution[2]; i++) {
		temp->rawdata[i].data = buffer[i];
	}

	vmodel = new VoxelModel;
	vmodel->name = (char*)malloc(sizeof(char) * 256);
	strcpy(vmodel->name, "Model_0");
	for (int i = 0; i < 3; i++) {
		vmodel->resolution[i] = 4096;
		vmodel->voxelsize[i] = 1;
	}
	vmodel->number_of_voxel_data = 1;

	//Creat root object
	vmodel->root_vobj = new VoxelObject;
	vmodel->root_vobj->name = (char*)malloc(sizeof(char) * 256);
	strcpy(vmodel->root_vobj->name, "Object_0");
	for (int i = 0; i < 3; i++) {
		vmodel->root_vobj->max_bound[0] = 4096;
		vmodel->root_vobj->min_bound[0] = 0;
	}
	vmodel->root_vobj->number_of_child = 3;
	
	(VoxelObject**)vmodel->root_vobj->child = (VoxelObject **)malloc(sizeof(VoxelObject *) * 3);

	VoxelObject * tempobj_1 = new VoxelObject;
	tempobj_1->name = (char*)malloc(sizeof(char) * 256);
	strcpy(tempobj_1->name, "Object_1");
	for (int i = 0; i < 3; i++) {
		tempobj_1->max_bound[0] = 2048;
		tempobj_1->min_bound[0] = 0;
	}
	tempobj_1->number_of_child = 1;
	tempobj_1->vd = temp;

	VoxelObject * tempobj_2 = new VoxelObject;
	tempobj_2->name = (char*)malloc(sizeof(char) * 256);
	strcpy(tempobj_2->name, "Object_2");
	for (int i = 0; i < 3; i++) {
		tempobj_2->max_bound[0] = 4096;
		tempobj_2->min_bound[0] = 2048;
	}
	tempobj_2->number_of_child = 1;
	tempobj_2->vd = temp;

	VoxelObject * tempobj_3 = new VoxelObject;
	tempobj_3->name = (char*)malloc(sizeof(char) * 256);
	strcpy(tempobj_3->name, "Object_3");
	for (int i = 0; i < 3; i++) {
		tempobj_3->max_bound[0] = 4096;
		tempobj_3->min_bound[0] = 0;
	}
	tempobj_3->number_of_child = 1;
	tempobj_3->vd = temp;

	vmodel->root_vobj->child[0] = tempobj_1;
	vmodel->root_vobj->child[1] = tempobj_2;
	vmodel->root_vobj->child[2] = tempobj_3;

	






	QString filename = QFileDialog::getSaveFileName(this, tr("Save File"),
		"untitled.vdat",
		tr("(*.vdat)"));
	if (filename.length() != 0) {
		QByteArray ba = filename.toLatin1();
		const char *fileLocationStr = ba.data();

		FILE* vdatafile;
		vdatafile = fopen(fileLocationStr, "wb");

		fwrite(resolution, sizeof(int), 3, vdatafile);
		fwrite(voxelsize, sizeof(float), 3, vdatafile);
		
		fwrite(&isbitcompress, sizeof(bool), 1, vdatafile);
		fwrite(buffer, sizeof(unsigned char), resolution[0] * resolution[1] * resolution[2], vdatafile);
		
		fclose(vdatafile);
		return;
	}
	else {
		QMessageBox::information(this, tr("Warning"), "Failed to save the file.");
	}



	
}

void MainWindow::openVD(const char* fileName) 
{
	int resolution[3];
	float voxelsize[3];
	bool isbitcompress;
	unsigned char * buffer;

	FILE * vdatafile = fopen(fileName, "rb");
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

void MainWindow::openVM(const char* fileName)
{
	VoxelModel* temp;

	FILE * vmodelfile = fopen(fileName, "r");
	if (vmodelfile != NULL) {






	}

	fclose(vmodelfile);

	return;
}
