#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{

}

void MainWindow::on_actionOpen_triggered()
{

}

void MainWindow::on_actionImport_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(
		this,
		tr("Open Voxel Model"),
		"",
		tr("Voxel Files (*.vdat *.vo *.vm)")
	);

	QByteArray ba = fileName.toLatin1();
	const char *fileLocationStr = ba.data();
	printf("Info file location:%s\n", fileLocationStr);







}

void MainWindow::on_actionExport_triggered()
{

	int resolution[3] = { 256,256,256 };
	float voxelsize[3] = { 1.0,0.5,1.0 };
	bool isbit = 1;
	char* buffer = (char *)calloc(sizeof(char), 256 * 256 * 256);

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
		"untitled.vdata",
		tr("(*.vdata)"));
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
