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

	ui->voxeltreeWidget->setColumnCount(1);
	


}

MainWindow::~MainWindow()
{

}

void MainWindow::makeTreeList()
{



}

void MainWindow::addInTreeList_VD(QTreeWidgetItem * parent, vdata_t vdata)
{
	QTreeWidgetItem * itm = new QTreeWidgetItem();


	parent->addChild(itm);
}

void MainWindow::addInTreeList_VD(vdata_t vdata)
{
	QTreeWidgetItem * itm = new QTreeWidgetItem(ui->voxeltreeWidget);
	if (vdata.name!=NULL) {
		itm->setText(0, QString(vdata.name));
	}
	else {
		itm->setText(0, "vdata");
	}
}

void MainWindow::addInTreeList_VO(QTreeWidgetItem * parent, vobj_t vobject)
{
	QTreeWidgetItem * itm = new QTreeWidgetItem();
	

}

void MainWindow::addInTreeList_VO(vobj_t vobject)
{
	QTreeWidgetItem * itm = new QTreeWidgetItem(ui->voxeltreeWidget);


}

void MainWindow::addInTreeList_VM(vmodel_t vmodel)
{
	QTreeWidgetItem * itm = new QTreeWidgetItem(ui->voxeltreeWidget);



}

void MainWindow::on_actionOpen_triggered()
{
	QString filename = QFileDialog::getOpenFileName(
		this,
		tr("Open Voxel Model"),
		"",
		tr("Voxel Files (*.vdat *.vo *.vm)")
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
		if (!strcmp(fe, ".vo")) {
			openVO(str);
		}
		if (!strcmp(fe, ".vm")) {
			openVM(str);
		}
	}
}

void MainWindow::on_actionImport_triggered()
{



}

void MainWindow::on_actionExport_triggered()
{

	int resolution[3] = { 256,256,256 };
	float voxelsize[3] = { 1.0,0.5,1.0 };
	bool isbit = 1;
	char* buffer = (char *)calloc(sizeof(char), 256 * 256 * 256);

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
		"untitled.vdat",
		tr("(*.vdat)"));
	if (fileName.length() != 0) {
		QByteArray ba = fileName.toLatin1();
		const char *fileLocationStr = ba.data();

		FILE* vdatafile;
		vdatafile = fopen(fileLocationStr, "wb");

		fwrite(resolution, sizeof(int), sizeof(resolution), vdatafile);
		fwrite(voxelsize, sizeof(float), sizeof(voxelsize), vdatafile);
		
		fwrite(&isbit, sizeof(bool), 1, vdatafile);
		fwrite(buffer, sizeof(char), sizeof(buffer), vdatafile);
		
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

	FILE * vdata = fopen(fileName, "r");
	if (vdata!=NULL) {
		fread(resolution, sizeof(resolution), sizeof(int), vdata);
		fread(voxelsize, sizeof(voxelsize), sizeof(float), vdata);
		fread(&isbitcompress, 1, sizeof(bool), vdata);

		(unsigned char *)buffer = (unsigned char *)calloc(resolution[0] * resolution[1] * resolution[2], sizeof(char));

		fread(buffer, sizeof(buffer), sizeof(char), vdata);
	}
	fclose(vdata);

	/*
	cout << resolution[0] << ", " << resolution[1] << ", " << resolution[2] << endl;
	cout << voxelsize[0] << ", " << voxelsize[1] << ", " << voxelsize[2] << endl;
	cout << isbit << endl;
	*/

	//Creat a vdata_t
	vdata_t* temp;
	for (int i = 0; i < 3;i++) {
		temp->resolution[i] = resolution[i];
		temp->voxelsize[i] = voxelsize[i];
	}
	temp->isbitcompress = isbitcompress;
	(voxel_t *)temp->rawdata = (voxel_t *)calloc(resolution[0] * resolution[1] * resolution[2], sizeof(char));
	for (int i = 0; i < resolution[0] * resolution[1] * resolution[2]; i++) {
		(temp->rawdata + i)->data = buffer[i];
	}

	//
	(vdata_t **)vda = (vdata_t **)realloc(vda, (sizeof(vda) + 1) * sizeof(vdata_t));
	vda[sizeof(vda) - 1] = temp;


	free(buffer);

	return;
}

void MainWindow::openVO(const char* fileName) 
{
	vobj_t* temp;

	FILE * vdata = fopen(fileName, "r");
	if (vdata != NULL) {






	}

	fclose(vdata);

	return;
}

void MainWindow::openVM(const char* fileName)
{
	vmodel_t* temp;

	FILE * vdata = fopen(fileName, "r");
	if (vdata != NULL) {






	}

	fclose(vdata);

	return;
}
