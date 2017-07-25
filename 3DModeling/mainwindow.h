#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "voxel.h"
#include "mytreewidgetitem.h"
#include "mylistwidgetitem.h"

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
	void addInDataList_VO(VoxelObject * vo);

	void addInTreeList_VD(MyTreeWidgetItem * parent, VoxelData* vdata);
	void addInTreeList_VO(MyTreeWidgetItem * parent, VoxelObject* vobject);
	void addInTreeList_VM(VoxelModel* vmodel);

	void openVD(const char* filepath);
	void openVD(const char* filepath, VoxelData* vd);
	void openVO(const char* filepath, FILE * vmodelfile, VoxelObject* vo);
	void openVM(const char* filepath);

	void writeVO(const char* filepath,FILE* vmodelfile,VoxelObject* vo);
	void writeVD(const char* filepath,VoxelData* vd);

	void setAttribute(VoxelData*);
	void setAttribute(VoxelObject*);
	void setAttribute(VoxelModel*);

private slots:
    void on_actionExport_triggered();
    void on_actionOpen_triggered();

	void setAttribute(QTreeWidgetItem *, int);
	void setAttribute(QListWidgetItem *);

signals:
	

};

#endif // MAINWINDOW_H
