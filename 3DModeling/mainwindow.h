#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "voxel.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	VoxelModel vmodel;

private:
	Ui::MainWindow *ui;



public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	void makeTreeList();
	void addInTreeList_VD(QTreeWidgetItem * parent, VoxelData vdata);
	void addInTreeList_VD(VoxelData vdata);
	void addInTreeList_VO(QTreeWidgetItem * parent, VoxelObject vobject);
	void addInTreeList_VO(VoxelObject vobject);
	void addInTreeList_VM(VoxelModel);

	void openVD(const char* fileName);
	void openVM(const char* fileName);

private slots:
    void on_actionExport_triggered();
    void on_actionOpen_triggered();

signals:
	

};

#endif // MAINWINDOW_H
