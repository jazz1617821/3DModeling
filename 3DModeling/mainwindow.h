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
	void makeTestFile(void);

	void addInDataList_VO(vobj_t * vo);

	void addInTreeList_VD(MyTreeWidgetItem * parent, ditem_t* dataItem);
	void addInTreeList_VO(MyTreeWidgetItem * parent, vobj_t* vobject);
	void addInTreeList_VM(vmodel_t* vmodel);

	void openVD(const char* filepath);
	void openVD(const char* filepath, vdata_t* vd);
	void openVO(const char* filepath, FILE * vmodelfile, vobj_t* vo);
	void openVM(const char* filepath);

	void writeVO(const char* filepath,FILE* vmodelfile, vobj_t* vo);
	void writeVD(const char* filepath, vdata_t* vd);

	void setAttribute(vdata_t*);
	void setAttribute(vobj_t*);
	void setAttribute(vmodel_t*);

	//signal trigger function
	void finishLoadVModel();

private slots:
    void on_actionSave_triggered();
    void on_actionOpen_triggered();

	void setAttribute(QTreeWidgetItem *, int);
	void setAttribute(QListWidgetItem *);

signals:
	void sentVModelPtr(vmodel_t*);
	void sentVDataPtr(vdata_t*);

};

#endif // MAINWINDOW_H
