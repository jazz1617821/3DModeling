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
	vdata_t ** vda;

private:
	Ui::MainWindow *ui;



public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	void makeTreeList();
	void addInTreeList_VD(QTreeWidgetItem * parent, vdata_t vdata);
	void addInTreeList_VD(vdata_t vdata);
	void addInTreeList_VO(QTreeWidgetItem * parent, vobj_t vobject);
	void addInTreeList_VO(vobj_t vobject);
	void addInTreeList_VM(vmodel_t );

	void openVD(const char* fileName);
	void openVO(const char* fileName);
	void openVM(const char* fileName);

private slots:
    void on_actionImport_triggered();
    void on_actionExport_triggered();
    void on_actionOpen_triggered();

signals:
	

};

#endif // MAINWINDOW_H
