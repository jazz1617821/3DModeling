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

	//ui->voxeltreeWidget->setColumnCount(1);
	


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
	bool isbit = 1;
	unsigned char* buffer = (unsigned char *)calloc(sizeof(unsigned char), resolution[0] * resolution[1] * resolution[2]);

	for (int i = 0; i < resolution[0] * resolution[1] * resolution[2]; i++) {
		buffer[i] = rand() % 256;
	}

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
		"untitled.vdat",
		tr("(*.vdat)"));
	if (fileName.length() != 0) {
		QByteArray ba = fileName.toLatin1();
		const char *fileLocationStr = ba.data();

		FILE* vdatafile;
		vdatafile = fopen(fileLocationStr, "wb");

		fwrite(resolution, sizeof(int), 3, vdatafile);
		fwrite(voxelsize, sizeof(float), 3, vdatafile);
		
		fwrite(&isbit, sizeof(bool), 1, vdatafile);
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

	FILE * vdata = fopen(fileName, "r");
	if (vdata != NULL) {






	}

	fclose(vdata);

	return;
}
