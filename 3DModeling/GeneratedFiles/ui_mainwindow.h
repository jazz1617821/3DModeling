/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionImport;
    QAction *actionExport;
    QAction *actionOpen;
    QWidget *centralWidget;
    QGridLayout *gridLayout_5;
    QSplitter *splitter;
    QTreeView *treeView;
    QWidget *widget_2;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *viewWidgetLayout;
    QGridLayout *gridLayout_2;
    QOpenGLWidget *viewWidget;
    QWidget *editWidgetLayout;
    QGridLayout *gridLayout_7;
    QGridLayout *gridLayout_3;
    QOpenGLWidget *y_axis_viewwidget;
    QOpenGLWidget *threedimesion_viewwidget;
    QOpenGLWidget *openGLWidget_4;
    QOpenGLWidget *openGLWidget_5;
    QToolBox *toolBox;
    QWidget *ioWidget;
    QGridLayout *gridLayout_4;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QWidget *morphologicalWidget;
    QWidget *modellogicalWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(967, 625);
        actionImport = new QAction(MainWindow);
        actionImport->setObjectName(QStringLiteral("actionImport"));
        actionExport = new QAction(MainWindow);
        actionExport->setObjectName(QStringLiteral("actionExport"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_5 = new QGridLayout(centralWidget);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        treeView = new QTreeView(splitter);
        treeView->setObjectName(QStringLiteral("treeView"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(treeView->sizePolicy().hasHeightForWidth());
        treeView->setSizePolicy(sizePolicy);
        treeView->setMinimumSize(QSize(0, 0));
        treeView->setMaximumSize(QSize(200, 16777215));
        splitter->addWidget(treeView);
        widget_2 = new QWidget(splitter);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy1);
        gridLayout = new QGridLayout(widget_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tabWidget = new QTabWidget(widget_2);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        viewWidgetLayout = new QWidget();
        viewWidgetLayout->setObjectName(QStringLiteral("viewWidgetLayout"));
        gridLayout_2 = new QGridLayout(viewWidgetLayout);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        viewWidget = new QOpenGLWidget(viewWidgetLayout);
        viewWidget->setObjectName(QStringLiteral("viewWidget"));
        sizePolicy1.setHeightForWidth(viewWidget->sizePolicy().hasHeightForWidth());
        viewWidget->setSizePolicy(sizePolicy1);
        viewWidget->setMouseTracking(true);

        gridLayout_2->addWidget(viewWidget, 0, 0, 1, 1);

        tabWidget->addTab(viewWidgetLayout, QString());
        editWidgetLayout = new QWidget();
        editWidgetLayout->setObjectName(QStringLiteral("editWidgetLayout"));
        gridLayout_7 = new QGridLayout(editWidgetLayout);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        y_axis_viewwidget = new QOpenGLWidget(editWidgetLayout);
        y_axis_viewwidget->setObjectName(QStringLiteral("y_axis_viewwidget"));
        sizePolicy1.setHeightForWidth(y_axis_viewwidget->sizePolicy().hasHeightForWidth());
        y_axis_viewwidget->setSizePolicy(sizePolicy1);
        y_axis_viewwidget->setMouseTracking(true);

        gridLayout_3->addWidget(y_axis_viewwidget, 0, 0, 1, 1);

        threedimesion_viewwidget = new QOpenGLWidget(editWidgetLayout);
        threedimesion_viewwidget->setObjectName(QStringLiteral("threedimesion_viewwidget"));
        sizePolicy1.setHeightForWidth(threedimesion_viewwidget->sizePolicy().hasHeightForWidth());
        threedimesion_viewwidget->setSizePolicy(sizePolicy1);
        threedimesion_viewwidget->setMouseTracking(true);

        gridLayout_3->addWidget(threedimesion_viewwidget, 0, 1, 1, 1);

        openGLWidget_4 = new QOpenGLWidget(editWidgetLayout);
        openGLWidget_4->setObjectName(QStringLiteral("openGLWidget_4"));
        sizePolicy1.setHeightForWidth(openGLWidget_4->sizePolicy().hasHeightForWidth());
        openGLWidget_4->setSizePolicy(sizePolicy1);
        openGLWidget_4->setMouseTracking(true);

        gridLayout_3->addWidget(openGLWidget_4, 1, 0, 1, 1);

        openGLWidget_5 = new QOpenGLWidget(editWidgetLayout);
        openGLWidget_5->setObjectName(QStringLiteral("openGLWidget_5"));
        sizePolicy1.setHeightForWidth(openGLWidget_5->sizePolicy().hasHeightForWidth());
        openGLWidget_5->setSizePolicy(sizePolicy1);
        openGLWidget_5->setMouseTracking(true);

        gridLayout_3->addWidget(openGLWidget_5, 1, 1, 1, 1);


        gridLayout_7->addLayout(gridLayout_3, 0, 0, 1, 1);

        toolBox = new QToolBox(editWidgetLayout);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(toolBox->sizePolicy().hasHeightForWidth());
        toolBox->setSizePolicy(sizePolicy2);
        toolBox->setMinimumSize(QSize(0, 0));
        toolBox->setMaximumSize(QSize(16777215, 500));
        ioWidget = new QWidget();
        ioWidget->setObjectName(QStringLiteral("ioWidget"));
        ioWidget->setGeometry(QRect(0, 0, 646, 69));
        gridLayout_4 = new QGridLayout(ioWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        radioButton = new QRadioButton(ioWidget);
        radioButton->setObjectName(QStringLiteral("radioButton"));

        gridLayout_4->addWidget(radioButton, 0, 0, 1, 1);

        radioButton_2 = new QRadioButton(ioWidget);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));

        gridLayout_4->addWidget(radioButton_2, 1, 0, 1, 1);

        toolBox->addItem(ioWidget, QStringLiteral("Insert , Delete"));
        morphologicalWidget = new QWidget();
        morphologicalWidget->setObjectName(QStringLiteral("morphologicalWidget"));
        morphologicalWidget->setGeometry(QRect(0, 0, 530, 82));
        toolBox->addItem(morphologicalWidget, QStringLiteral("Morphological"));
        modellogicalWidget = new QWidget();
        modellogicalWidget->setObjectName(QStringLiteral("modellogicalWidget"));
        modellogicalWidget->setGeometry(QRect(0, 0, 530, 82));
        toolBox->addItem(modellogicalWidget, QStringLiteral("Model logical"));

        gridLayout_7->addWidget(toolBox, 1, 0, 1, 1);

        tabWidget->addTab(editWidgetLayout, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        splitter->addWidget(widget_2);

        gridLayout_5->addWidget(splitter, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 967, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionImport);
        menuFile->addAction(actionExport);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        toolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "mainwindow", Q_NULLPTR));
        actionImport->setText(QApplication::translate("MainWindow", "Import", Q_NULLPTR));
        actionExport->setText(QApplication::translate("MainWindow", "Export", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(viewWidgetLayout), QApplication::translate("MainWindow", "View Model", Q_NULLPTR));
        radioButton->setText(QApplication::translate("MainWindow", "RadioButton", Q_NULLPTR));
        radioButton_2->setText(QApplication::translate("MainWindow", "RadioButton", Q_NULLPTR));
        toolBox->setItemText(toolBox->indexOf(ioWidget), QApplication::translate("MainWindow", "Insert , Delete", Q_NULLPTR));
        toolBox->setItemText(toolBox->indexOf(morphologicalWidget), QApplication::translate("MainWindow", "Morphological", Q_NULLPTR));
        toolBox->setItemText(toolBox->indexOf(modellogicalWidget), QApplication::translate("MainWindow", "Model logical", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(editWidgetLayout), QApplication::translate("MainWindow", "Edit Model", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
