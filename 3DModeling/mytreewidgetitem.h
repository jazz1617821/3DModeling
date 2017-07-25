#pragma once
#include <QTreeWidgetItem>
#include "voxel.h"

class MyTreeWidgetItem : public QTreeWidgetItem {

public:
	VoxelData* vd = NULL;
	VoxelObject* vo = NULL;
	VoxelModel* vm = NULL;

private:



public:
	MyTreeWidgetItem(QTreeWidget * parent = 0, int type = Type);
	~MyTreeWidgetItem();

private:
	



};
