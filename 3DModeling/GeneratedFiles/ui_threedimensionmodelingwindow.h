/********************************************************************************
** Form generated from reading UI file 'threedimensionmodelingwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_THREEDIMENSIONMODELINGWINDOW_H
#define UI_THREEDIMENSIONMODELINGWINDOW_H

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
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>
#include "editwidget.h"
#include "viewwidget.h"

QT_BEGIN_NAMESPACE

class Ui_TDMMainWindow
{
public:
    QAction *actionSave;
    QAction *actionOpen;
    QAction *actionImport;
    QAction *actionExport;
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
    QGridLayout *gridLayout_12;
    EditWidget *editWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;

    void setupUi(QMainWindow *TDMMainWindow)
    {
        if (TDMMainWindow->objectName().isEmpty())
            TDMMainWindow->setObjectName(QStringLiteral("TDMMainWindow"));
        TDMMainWindow->resize(773, 606);
        TDMMainWindow->setMaximumSize(QSize(16777215, 16777215));
        actionSave = new QAction(TDMMainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionOpen = new QAction(TDMMainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionImport = new QAction(TDMMainWindow);
        actionImport->setObjectName(QStringLiteral("actionImport"));
        actionExport = new QAction(TDMMainWindow);
        actionExport->setObjectName(QStringLiteral("actionExport"));
        centralWidget = new QWidget(TDMMainWindow);
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
        voxeltreeWidget->setColumnCount(1);
        voxeltreeWidget->header()->setVisible(false);

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
        attributetableWidget->horizontalHeader()->setVisible(false);
        attributetableWidget->verticalHeader()->setVisible(false);

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
        gridLayout_12 = new QGridLayout(editWidgetLayout);
        gridLayout_12->setSpacing(6);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        editWidget = new EditWidget(editWidgetLayout);
        editWidget->setObjectName(QStringLiteral("editWidget"));
        sizePolicy.setHeightForWidth(editWidget->sizePolicy().hasHeightForWidth());
        editWidget->setSizePolicy(sizePolicy);
        editWidget->setMouseTracking(true);

        gridLayout_12->addWidget(editWidget, 0, 0, 1, 1);

        view_tabWidget->addTab(editWidgetLayout, QString());

        gridLayout->addWidget(view_tabWidget, 0, 1, 1, 1);

        splitter->addWidget(rightWidgetLayout);

        gridLayout_3->addWidget(splitter, 0, 0, 1, 1);

        TDMMainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TDMMainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 773, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        TDMMainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionImport);
        menuFile->addAction(actionExport);

        retranslateUi(TDMMainWindow);

        tabWidget->setCurrentIndex(0);
        view_tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(TDMMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *TDMMainWindow)
    {
        TDMMainWindow->setWindowTitle(QApplication::translate("TDMMainWindow", "3DModeling", 0));
        actionSave->setText(QApplication::translate("TDMMainWindow", "Save", 0));
        actionOpen->setText(QApplication::translate("TDMMainWindow", "Open", 0));
        actionImport->setText(QApplication::translate("TDMMainWindow", "Import", 0));
        actionExport->setText(QApplication::translate("TDMMainWindow", "Export", 0));
        tabWidget->setTabText(tabWidget->indexOf(widget), QApplication::translate("TDMMainWindow", "Voxel Model", 0));
        tabWidget->setTabText(tabWidget->indexOf(widget1), QApplication::translate("TDMMainWindow", "Voxel Data", 0));
        AttributeLabel->setText(QApplication::translate("TDMMainWindow", "Attributes", 0));
        view_tabWidget->setTabText(view_tabWidget->indexOf(viewWidgetLayout), QApplication::translate("TDMMainWindow", "View Model", 0));
        view_tabWidget->setTabText(view_tabWidget->indexOf(editWidgetLayout), QApplication::translate("TDMMainWindow", "Edit Model", 0));
        menuFile->setTitle(QApplication::translate("TDMMainWindow", "File", 0));
        menuHelp->setTitle(QApplication::translate("TDMMainWindow", "Help", 0));
    } // retranslateUi

};

namespace Ui {
    class TDMMainWindow: public Ui_TDMMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_THREEDIMENSIONMODELINGWINDOW_H
