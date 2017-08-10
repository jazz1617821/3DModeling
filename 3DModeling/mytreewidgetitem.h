#pragma once
#include <QTreeWidgetItem>
#include "voxel.h"

class MyTreeWidgetItem : public QTreeWidgetItem {

public:
	ditem_t* dataItem = NULL;
	vobj_t* vo = NULL;
	vmodel_t* vm = NULL;

private:



public:
	MyTreeWidgetItem(QTreeWidget * parent = 0, int type = Type);
	~MyTreeWidgetItem();

private:
	



};
