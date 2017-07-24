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

	VoxelData* tempData_1 = new VoxelData;
	tempData_1->name = (char*)malloc(sizeof(char) * 256);
	strcpy(tempData_1->name, "Data_0");
	for (int i = 0; i < 3; i++) {
		tempData_1->resolution[i] = resolution[i];
		tempData_1->voxelsize[i] = voxelsize[i];
	}
	tempData_1->isbitcompress = isbitcompress;

	(Voxel *)tempData_1->rawdata = (Voxel *)calloc(resolution[0] * resolution[1] * resolution[2], sizeof(Voxel));

	for (int i = 0; i < resolution[0] * resolution[1] * resolution[2]; i++) {
		tempData_1->rawdata[i].data = buffer[i];
	}

	VoxelData* tempData_2 = new VoxelData;
	tempData_2->name = (char*)malloc(sizeof(char) * 256);
	strcpy(tempData_2->name, "Data_1");
	for (int i = 0; i < 3; i++) {
		tempData_2->resolution[i] = resolution[i];
		tempData_2->voxelsize[i] = voxelsize[i];
	}
	tempData_2->isbitcompress = isbitcompress;

	(Voxel *)tempData_2->rawdata = (Voxel *)calloc(resolution[0] * resolution[1] * resolution[2], sizeof(Voxel));

	for (int i = 0; i < resolution[0] * resolution[1] * resolution[2]; i++) {
		tempData_2->rawdata[i].data = buffer[i];
	}

	VoxelData* tempData_3 = new VoxelData;
	tempData_3->name = (char*)malloc(sizeof(char) * 256);
	strcpy(tempData_3->name, "Data_2");
	for (int i = 0; i < 3; i++) {
		tempData_3->resolution[i] = resolution[i];
		tempData_3->voxelsize[i] = voxelsize[i];
	}
	tempData_3->isbitcompress = isbitcompress;

	(Voxel *)tempData_3->rawdata = (Voxel *)calloc(resolution[0] * resolution[1] * resolution[2], sizeof(Voxel));

	for (int i = 0; i < resolution[0] * resolution[1] * resolution[2]; i++) {
		tempData_3->rawdata[i].data = buffer[i];
	}

	free(buffer);


	vmodel = new VoxelModel;
	vmodel->name = (char*)malloc(sizeof(char) * 256);
	strcpy(vmodel->name, "Model_0");
	for (int i = 0; i < 3; i++) {
		vmodel->resolution[i] = 4096;
		vmodel->voxelsize[i] = 1;
	}
	vmodel->number_of_voxel_data = 3;

	//Creat root object
	vmodel->root_vobj = new VoxelObject;
	vmodel->root_vobj->name = (char*)malloc(sizeof(char) * 256);
	strcpy(vmodel->root_vobj->name, "Object_0");
	for (int i = 0; i < 3; i++) {
		vmodel->root_vobj->max_bound[i] = 4096;
		vmodel->root_vobj->min_bound[i] = 0;
	}
	vmodel->root_vobj->number_of_child = 3;
	
	(VoxelObject**)vmodel->root_vobj->child = (VoxelObject **)malloc(sizeof(VoxelObject *) * 3);

	VoxelObject * tempobj_1 = new VoxelObject;
	tempobj_1->name = (char*)malloc(sizeof(char) * 256);
	strcpy(tempobj_1->name, "Object_1");
	for (int i = 0; i < 3; i++) {
		tempobj_1->max_bound[i] = 2048;
		tempobj_1->min_bound[i] = 0;
	}
	tempobj_1->number_of_child = 1;
	tempobj_1->vd = tempData_1;

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

	
	QString filename = QFileDialog::getSaveFileName(this, tr("Save File"),
		"untitled",
		tr("Model Files (*.vm);; Data Files (*.vdat)"));
	QByteArray ba = filename.toLatin1();
	const char *str = ba.data();
	const char *fe;
	fe = strrchr(str, '.');

	if (!filename.isEmpty()) {
		if (!strcmp(fe,".vm")) {
			//export vmodel

			FILE* vmodelfile = fopen(str, "w");

			//Write vmodel 
			fprintf(vmodelfile, "Voxel Model:\n");
			fprintf(vmodelfile, "Name:%s\n", vmodel->name);
			fprintf(vmodelfile, "Resolution:%dx%dx%d\n",vmodel->resolution[0], vmodel->resolution[1], vmodel->resolution[2]);
			fprintf(vmodelfile, "Voxelsize:%.2f:%.2f:%.2f\n", vmodel->voxelsize[0], vmodel->voxelsize[1], vmodel->voxelsize[2]);
			fprintf(vmodelfile, "Number of voxel data:%d\n", vmodel->number_of_voxel_data);



			//DFS Write Voxel Object
			writeVO(str,vmodelfile, vmodel->root_vobj);

			//
			fclose(vmodelfile);
			return;
		}
		else if(!strcmp(fe, ".vdat")) {
			//export vdata



		}
	}
	else {
		QMessageBox::information(this, tr("Warning"), "Failed to save the file.");
	}



	
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

void MainWindow::openVD(const char * filepath, VoxelData * vd)
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

void MainWindow::openVO(const char* filepath,FILE * vmodelfile, VoxelObject* vo)
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
	vmodel = new VoxelModel;

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

		vmodel->root_vobj = new VoxelObject;
		openVO(filepath, vmodelfile, vmodel->root_vobj);
	}

	fclose(vmodelfile);

	addInTreeList_VM(vmodel);

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

void MainWindow::writeVD(const char* filepath, VoxelData * vd)
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
		fwrite(vd->resolution, sizeof(int), 3, vdatafile);
		fwrite(vd->voxelsize, sizeof(float), 3, vdatafile);
		fwrite(&vd->isbitcompress, sizeof(int), 1, vdatafile);
		for (int i = 0; i < vd->resolution[0] * vd->resolution[1] * vd->resolution[2]; i++) {
			fwrite(&vd->rawdata[i].data, sizeof(unsigned char), 1, vdatafile);
		}

		fclose(vdatafile);
	}

	free(str);

	return;
}
