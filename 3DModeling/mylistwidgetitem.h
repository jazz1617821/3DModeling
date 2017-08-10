#pragma once
#include <QListWidgetItem>
#include "voxel.h"

class MyListWidgetItem : public QListWidgetItem {

public:
	ditem_t* dataItem = NULL;

private:



public:
	MyListWidgetItem(QListWidget * parent = Q_NULLPTR, int type = Type);
	~MyListWidgetItem();

private:
	
};
