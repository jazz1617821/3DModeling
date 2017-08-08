#include "enqlist.h"
#include <stdio.h>

int initEnqDList(enqdlist_t* item)
{
	if (item == NULL) return false;

	item->flink = item;
	item->blink = item;

	return true;
}

int isEnqed(enqdlist_t* item)
{
	if (item->flink == item)
		return false;
	return true;
}

int isEnqEmpty(enqdlist_t* ls)
{
	if (ls->flink == ls && ls->blink == ls)
		return true;
	return false;
}

int addEnqHead(enqdlist_t* ls, enqdlist_t* item)
{
	item->flink = ls;
	item->blink = ls->blink;
	ls->blink = item;

	return true;
}

int addEnqTail(enqdlist_t* ls, enqdlist_t* item)
{
	item->flink = ls->flink;
	item->blink = ls;
	ls->flink = item;

	return true;
}

int addEnqBefore(enqdlist_t* target, enqdlist_t* item)
{
	item->flink = target->flink;
	item->blink = target;
	target->flink = item;

	return true;
}

int addEnqAfter(enqdlist_t* target, enqdlist_t* item)
{
	item->flink = target;
	item->blink = target->blink;
	target->blink = item;

	return true;
}

int removeEnqHead(enqdlist_t* ls, enqdlist_t** item)
{
	(*item) = ls->blink;
	(*item)->flink = *item;
	(*item)->blink = *item;
	ls->blink = ls->blink->blink;

	return true;
}

int removeEnqTail(enqdlist_t* ls, enqdlist_t** item)
{
	(*item) = ls->flink;
	(*item)->flink = *item;
	(*item)->blink = *item;
	ls->flink = ls->flink->flink;

	return true;
}

int removeEnqItem(enqdlist_t* item)
{
	item->flink->blink = item->blink;
	item->blink->flink = item->flink;

	return true;
}
