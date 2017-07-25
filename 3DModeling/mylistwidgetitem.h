#pragma once
#include <QListWidgetItem>
#include "voxel.h"

class MyListWidgetItem : public QListWidgetItem {

public:
	VoxelData* vd = NULL;

private:



public:
	MyListWidgetItem(QListWidget * parent = Q_NULLPTR, int type = Type);
	~MyListWidgetItem();

private:
	
};
