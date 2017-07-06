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
    QGridLayout *gridLayout;
    QTreeView *treeView;
    QTabWidget *tabWidget;
    QWidget *viewWidgetLayout;
    QGridLayout *gridLayout_2;
    QOpenGLWidget *viewWidget;
    QWidget *editWidgetLayout;
    QGridLayout *gridLayout_3;
    QOpenGLWidget *openGLWidget_2;
    QOpenGLWidget *openGLWidget_4;
    QOpenGLWidget *openGLWidget_3;
    QToolBox *toolBox;
    QWidget *ioWidget;
    QGridLayout *gridLayout_4;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QWidget *morphologicalWidget;
    QWidget *modellogicalWidget;
    QOpenGLWidget *openGLWidget_5;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(814, 592);
        actionImport = new QAction(MainWindow);
        actionImport->setObjectName(QStringLiteral("actionImport"));
        actionExport = new QAction(MainWindow);
        actionExport->setObjectName(QStringLiteral("actionExport"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        treeView = new QTreeView(centralWidget);
        treeView->setObjectName(QStringLiteral("treeView"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(treeView->sizePolicy().hasHeightForWidth());
        treeView->setSizePolicy(sizePolicy);
        treeView->setMinimumSize(QSize(0, 0));
        treeView->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(treeView, 0, 0, 1, 1);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        viewWidgetLayout = new QWidget();
        viewWidgetLayout->setObjectName(QStringLiteral("viewWidgetLayout"));
        gridLayout_2 = new QGridLayout(viewWidgetLayout);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        viewWidget = new QOpenGLWidget(viewWidgetLayout);
        viewWidget->setObjectName(QStringLiteral("viewWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(viewWidget->sizePolicy().hasHeightForWidth());
        viewWidget->setSizePolicy(sizePolicy1);
        viewWidget->setMouseTracking(true);

        gridLayout_2->addWidget(viewWidget, 0, 0, 1, 1);

        tabWidget->addTab(viewWidgetLayout, QString());
        editWidgetLayout = new QWidget();
        editWidgetLayout->setObjectName(QStringLiteral("editWidgetLayout"));
        gridLayout_3 = new QGridLayout(editWidgetLayout);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        openGLWidget_2 = new QOpenGLWidget(editWidgetLayout);
        openGLWidget_2->setObjectName(QStringLiteral("openGLWidget_2"));
        sizePolicy1.setHeightForWidth(openGLWidget_2->sizePolicy().hasHeightForWidth());
        openGLWidget_2->setSizePolicy(sizePolicy1);
        openGLWidget_2->setMouseTracking(true);

        gridLayout_3->addWidget(openGLWidget_2, 0, 0, 1, 1);

        openGLWidget_4 = new QOpenGLWidget(editWidgetLayout);
        openGLWidget_4->setObjectName(QStringLiteral("openGLWidget_4"));
        sizePolicy1.setHeightForWidth(openGLWidget_4->sizePolicy().hasHeightForWidth());
        openGLWidget_4->setSizePolicy(sizePolicy1);
        openGLWidget_4->setMouseTracking(true);

        gridLayout_3->addWidget(openGLWidget_4, 1, 0, 1, 1);

        openGLWidget_3 = new QOpenGLWidget(editWidgetLayout);
        openGLWidget_3->setObjectName(QStringLiteral("openGLWidget_3"));
        sizePolicy1.setHeightForWidth(openGLWidget_3->sizePolicy().hasHeightForWidth());
        openGLWidget_3->setSizePolicy(sizePolicy1);
        openGLWidget_3->setMouseTracking(true);

        gridLayout_3->addWidget(openGLWidget_3, 0, 1, 1, 1);

        toolBox = new QToolBox(editWidgetLayout);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        toolBox->setMinimumSize(QSize(0, 0));
        ioWidget = new QWidget();
        ioWidget->setObjectName(QStringLiteral("ioWidget"));
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
        morphologicalWidget->setGeometry(QRect(0, 0, 566, 88));
        toolBox->addItem(morphologicalWidget, QStringLiteral("Morphological"));
        modellogicalWidget = new QWidget();
        modellogicalWidget->setObjectName(QStringLiteral("modellogicalWidget"));
        modellogicalWidget->setGeometry(QRect(0, 0, 566, 88));
        toolBox->addItem(modellogicalWidget, QStringLiteral("Model logical"));

        gridLayout_3->addWidget(toolBox, 2, 0, 1, 2);

        openGLWidget_5 = new QOpenGLWidget(editWidgetLayout);
        openGLWidget_5->setObjectName(QStringLiteral("openGLWidget_5"));
        sizePolicy1.setHeightForWidth(openGLWidget_5->sizePolicy().hasHeightForWidth());
        openGLWidget_5->setSizePolicy(sizePolicy1);
        openGLWidget_5->setMouseTracking(true);

        gridLayout_3->addWidget(openGLWidget_5, 1, 1, 1, 1);

        tabWidget->addTab(editWidgetLayout, QString());

        gridLayout->addWidget(tabWidget, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 814, 21));
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
