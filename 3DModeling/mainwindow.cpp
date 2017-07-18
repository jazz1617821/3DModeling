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

void MainWindow::makeTreeList()
{



}

void MainWindow::addInTreeList_VD(QTreeWidgetItem * parent, VoxelData vdata)
{
	QTreeWidgetItem * itm = new QTreeWidgetItem();


	parent->addChild(itm);
}

void MainWindow::addInTreeList_VD(VoxelData vdata)
{
	QTreeWidgetItem * itm = new QTreeWidgetItem(ui->voxeltreeWidget);
	if (vdata.name!=NULL) {
		itm->setText(0, QString(vdata.name));
	}
	else {
		itm->setText(0, "vdata");
	}
}

void MainWindow::addInTreeList_VO(QTreeWidgetItem * parent, VoxelObject vobject)
{
	QTreeWidgetItem * itm = new QTreeWidgetItem();
	

}

void MainWindow::addInTreeList_VO(VoxelObject vobject)
{
	QTreeWidgetItem * itm = new QTreeWidgetItem(ui->voxeltreeWidget);


}

void MainWindow::addInTreeList_VM(VoxelModel vmodel)
{
	QTreeWidgetItem * itm = new QTreeWidgetItem(ui->voxeltreeWidget);



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
	unsigned char* buffer = (unsigned char *)calloc(sizeof(unsigned char), 256 * 256 * 256);

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

		fwrite(resolution, sizeof(int), sizeof(resolution), vdatafile);
		fwrite(voxelsize, sizeof(float), sizeof(voxelsize), vdatafile);
		
		fwrite(&isbit, sizeof(bool), 1, vdatafile);
		fwrite(buffer, sizeof(unsigned char), sizeof(buffer), vdatafile);
		
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
	VoxelData temp;
	for (int i = 0; i < 3;i++) {
		temp.resolution[i] = resolution[i];
		temp.voxelsize[i] = voxelsize[i];
	}
	temp.isbitcompress = isbitcompress;

	(Voxel *)temp.rawdata = (Voxel *)calloc(resolution[0] * resolution[1] * resolution[2] , sizeof(Voxel));
	

	for (int i = 0; i < resolution[0] * resolution[1] * resolution[2];i++) {
		temp.rawdata[i].data = buffer[i];
	}

	free(buffer);













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
