/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
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
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>
#include "viewwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExport;
    QAction *actionOpen;
    QWidget *centralWidget;
    QGridLayout *gridLayout_3;
    QSplitter *splitter;
    QWidget *leftWidgetLayout;
    QGridLayout *gridLayout_6;
    QTabWidget *tabWidget;
    QWidget *widget;
    QGridLayout *gridLayout_5;
    QTreeWidget *voxeltreeWidget;
    QWidget *widget1;
    QGridLayout *gridLayout_8;
    QListWidget *datalistWidget;
    QLabel *AttributeLabel;
    QTableWidget *attributetableWidget;
    QWidget *rightWidgetLayout;
    QGridLayout *gridLayout;
    QTabWidget *view_tabWidget;
    QWidget *viewWidgetLayout;
    QGridLayout *gridLayout_2;
    ViewWidget *viewWidget;
    QWidget *editWidgetLayout;
    QGridLayout *gridLayout_7;
    QOpenGLWidget *editwidget;
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
        MainWindow->resize(773, 595);
        actionExport = new QAction(MainWindow);
        actionExport->setObjectName(QStringLiteral("actionExport"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Horizontal);
        leftWidgetLayout = new QWidget(splitter);
        leftWidgetLayout->setObjectName(QStringLiteral("leftWidgetLayout"));
        leftWidgetLayout->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(leftWidgetLayout->sizePolicy().hasHeightForWidth());
        leftWidgetLayout->setSizePolicy(sizePolicy1);
        leftWidgetLayout->setMinimumSize(QSize(250, 0));
        leftWidgetLayout->setMaximumSize(QSize(500, 16777215));
        leftWidgetLayout->setBaseSize(QSize(0, 0));
        leftWidgetLayout->setLayoutDirection(Qt::LeftToRight);
        leftWidgetLayout->setAutoFillBackground(false);
        gridLayout_6 = new QGridLayout(leftWidgetLayout);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        tabWidget = new QTabWidget(leftWidgetLayout);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        widget = new QWidget();
        widget->setObjectName(QStringLiteral("widget"));
        gridLayout_5 = new QGridLayout(widget);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        voxeltreeWidget = new QTreeWidget(widget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        voxeltreeWidget->setHeaderItem(__qtreewidgetitem);
        voxeltreeWidget->setObjectName(QStringLiteral("voxeltreeWidget"));

        gridLayout_5->addWidget(voxeltreeWidget, 0, 0, 1, 1);

        tabWidget->addTab(widget, QString());
        widget1 = new QWidget();
        widget1->setObjectName(QStringLiteral("widget1"));
        gridLayout_8 = new QGridLayout(widget1);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        datalistWidget = new QListWidget(widget1);
        datalistWidget->setObjectName(QStringLiteral("datalistWidget"));

        gridLayout_8->addWidget(datalistWidget, 0, 0, 1, 1);

        tabWidget->addTab(widget1, QString());

        gridLayout_6->addWidget(tabWidget, 0, 0, 1, 1);

        AttributeLabel = new QLabel(leftWidgetLayout);
        AttributeLabel->setObjectName(QStringLiteral("AttributeLabel"));

        gridLayout_6->addWidget(AttributeLabel, 1, 0, 1, 1);

        attributetableWidget = new QTableWidget(leftWidgetLayout);
        attributetableWidget->setObjectName(QStringLiteral("attributetableWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(attributetableWidget->sizePolicy().hasHeightForWidth());
        attributetableWidget->setSizePolicy(sizePolicy2);

        gridLayout_6->addWidget(attributetableWidget, 2, 0, 1, 1);

        splitter->addWidget(leftWidgetLayout);
        rightWidgetLayout = new QWidget(splitter);
        rightWidgetLayout->setObjectName(QStringLiteral("rightWidgetLayout"));
        sizePolicy.setHeightForWidth(rightWidgetLayout->sizePolicy().hasHeightForWidth());
        rightWidgetLayout->setSizePolicy(sizePolicy);
        rightWidgetLayout->setMinimumSize(QSize(500, 0));
        rightWidgetLayout->setLayoutDirection(Qt::LeftToRight);
        gridLayout = new QGridLayout(rightWidgetLayout);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        view_tabWidget = new QTabWidget(rightWidgetLayout);
        view_tabWidget->setObjectName(QStringLiteral("view_tabWidget"));
        sizePolicy.setHeightForWidth(view_tabWidget->sizePolicy().hasHeightForWidth());
        view_tabWidget->setSizePolicy(sizePolicy);
        viewWidgetLayout = new QWidget();
        viewWidgetLayout->setObjectName(QStringLiteral("viewWidgetLayout"));
        sizePolicy.setHeightForWidth(viewWidgetLayout->sizePolicy().hasHeightForWidth());
        viewWidgetLayout->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(viewWidgetLayout);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        viewWidget = new ViewWidget(viewWidgetLayout);
        viewWidget->setObjectName(QStringLiteral("viewWidget"));
        sizePolicy.setHeightForWidth(viewWidget->sizePolicy().hasHeightForWidth());
        viewWidget->setSizePolicy(sizePolicy);
        viewWidget->setMouseTracking(true);

        gridLayout_2->addWidget(viewWidget, 0, 0, 1, 1);

        view_tabWidget->addTab(viewWidgetLayout, QString());
        editWidgetLayout = new QWidget();
        editWidgetLayout->setObjectName(QStringLiteral("editWidgetLayout"));
        sizePolicy.setHeightForWidth(editWidgetLayout->sizePolicy().hasHeightForWidth());
        editWidgetLayout->setSizePolicy(sizePolicy);
        gridLayout_7 = new QGridLayout(editWidgetLayout);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        editwidget = new QOpenGLWidget(editWidgetLayout);
        editwidget->setObjectName(QStringLiteral("editwidget"));
        sizePolicy.setHeightForWidth(editwidget->sizePolicy().hasHeightForWidth());
        editwidget->setSizePolicy(sizePolicy);
        editwidget->setMouseTracking(true);

        gridLayout_7->addWidget(editwidget, 0, 0, 1, 1);

        toolBox = new QToolBox(editWidgetLayout);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(toolBox->sizePolicy().hasHeightForWidth());
        toolBox->setSizePolicy(sizePolicy3);
        toolBox->setMinimumSize(QSize(0, 0));
        toolBox->setMaximumSize(QSize(16777215, 500));
        ioWidget = new QWidget();
        ioWidget->setObjectName(QStringLiteral("ioWidget"));
        ioWidget->setGeometry(QRect(0, 0, 101, 56));
        sizePolicy.setHeightForWidth(ioWidget->sizePolicy().hasHeightForWidth());
        ioWidget->setSizePolicy(sizePolicy);
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
        morphologicalWidget->setGeometry(QRect(0, 0, 100, 30));
        sizePolicy.setHeightForWidth(morphologicalWidget->sizePolicy().hasHeightForWidth());
        morphologicalWidget->setSizePolicy(sizePolicy);
        toolBox->addItem(morphologicalWidget, QStringLiteral("Morphological"));
        modellogicalWidget = new QWidget();
        modellogicalWidget->setObjectName(QStringLiteral("modellogicalWidget"));
        modellogicalWidget->setGeometry(QRect(0, 0, 458, 69));
        sizePolicy.setHeightForWidth(modellogicalWidget->sizePolicy().hasHeightForWidth());
        modellogicalWidget->setSizePolicy(sizePolicy);
        toolBox->addItem(modellogicalWidget, QStringLiteral("Model logical"));

        gridLayout_7->addWidget(toolBox, 1, 0, 1, 1);

        view_tabWidget->addTab(editWidgetLayout, QString());

        gridLayout->addWidget(view_tabWidget, 0, 0, 1, 1);

        splitter->addWidget(rightWidgetLayout);

        gridLayout_3->addWidget(splitter, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 773, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionExport);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        view_tabWidget->setCurrentIndex(0);
        toolBox->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "mainwindow", 0));
        actionExport->setText(QApplication::translate("MainWindow", "Export", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        tabWidget->setTabText(tabWidget->indexOf(widget), QApplication::translate("MainWindow", "Voxel Model", 0));
        tabWidget->setTabText(tabWidget->indexOf(widget1), QApplication::translate("MainWindow", "Voxel Data", 0));
        AttributeLabel->setText(QApplication::translate("MainWindow", "Attributes", 0));
        view_tabWidget->setTabText(view_tabWidget->indexOf(viewWidgetLayout), QApplication::translate("MainWindow", "View Model", 0));
        radioButton->setText(QApplication::translate("MainWindow", "RadioButton", 0));
        radioButton_2->setText(QApplication::translate("MainWindow", "RadioButton", 0));
        toolBox->setItemText(toolBox->indexOf(ioWidget), QApplication::translate("MainWindow", "Insert , Delete", 0));
        toolBox->setItemText(toolBox->indexOf(morphologicalWidget), QApplication::translate("MainWindow", "Morphological", 0));
        toolBox->setItemText(toolBox->indexOf(modellogicalWidget), QApplication::translate("MainWindow", "Model logical", 0));
        view_tabWidget->setTabText(view_tabWidget->indexOf(editWidgetLayout), QApplication::translate("MainWindow", "Edit Model", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
