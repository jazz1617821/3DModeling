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

private slots:
    void on_actionImport_triggered();

    void on_actionExport_triggered();

    void on_actionOpen_triggered();

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
