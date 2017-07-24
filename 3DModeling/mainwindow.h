#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "voxel.h"
#include "mytreewidgetitem.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	VoxelModel* vmodel = NULL;

private:
	Ui::MainWindow *ui;



public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	void addInTreeList_VD(MyTreeWidgetItem * parent, VoxelData* vdata);
	void addInTreeList_VO(MyTreeWidgetItem * parent, VoxelObject* vobject);
	void addInTreeList_VM(VoxelModel* vmodel);

	void openVD(const char* filepath);
	void openVD(const char* filepath, VoxelData* vd);
	void openVO(const char* filepath, FILE * vmodelfile, VoxelObject* vo);
	void openVM(const char* filepath);

	void writeVO(const char* filepath,FILE* vmodelfile,VoxelObject* vo);
	void writeVD(const char* filepath,VoxelData* vd);

private slots:
    void on_actionExport_triggered();
    void on_actionOpen_triggered();

signals:
	

};

#endif // MAINWINDOW_H
