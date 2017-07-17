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
	MainWindow(QWidget *parent = 0);
	~MainWindow();
	void openVD(const char* fileName);
	void openVO(const char* fileName);
	void openVM(const char* fileName);

private slots:
    void on_actionImport_triggered();
    void on_actionExport_triggered();
    void on_actionOpen_triggered();

signals:
	

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
