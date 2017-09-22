#include "enqlist.h"
#include <stdio.h>

int initEnqDList(enqdlist_t *item)
{
	if (item == NULL) return false;

	item->flink = item;
	item->blink = item;

	return true;
}

int isEnqed(enqdlist_t *item)
{
	if (item->flink == item || item->blink == item)
		return false;
	return true;
}

int isEnqEmpty(enqdlist_t *ls)
{
	if (ls->flink == ls && ls->blink == ls)
		return true;
	return false;
}

int addEnqHead(enqdlist_t *ls, enqdlist_t *item)
{
	if (ls == NULL || item == NULL) {
		return false;
	}
	item->flink = ls->flink;
	item->blink = ls;
	ls->flink->blink = item;
	ls->flink = item;

	return true;
}

int addEnqTail(enqdlist_t  *ls, enqdlist_t *item)
{
	if (ls == NULL || item == NULL) {
		return false;
	}
	item->flink = ls;
	item->blink = ls->blink;
	ls->blink->flink = item;
	ls->blink = item;

	return true;
}

int addEnqBefore(enqdlist_t *target, enqdlist_t *item)
{
	if (target == NULL || item == NULL) {
		return false;
	}
	item->flink = target;
	item->blink = target->blink;
	target->blink->flink = item;
	target->blink = item;

	return true;
}

int addEnqAfter(enqdlist_t *target, enqdlist_t *item)
{
	if (target == NULL || item == NULL) {
		return false;
	}
	item->flink = target->flink;
	item->blink = target;
	target->flink->blink = item;
	target->flink = item;

	return true;
}

enqdlist_t *removeEnqHead(enqdlist_t* ls)
{
	enqdlist_t *item;

	if (isEnqEmpty(ls)) {
		return NULL;
	}
	item = ls->flink;
	removeEnqItem(item);
	item->flink = item;
	item->blink = item;

	return item;
}

enqdlist_t *removeEnqTail(enqdlist_t* ls)
{
	enqdlist_t *item;

	if (isEnqEmpty(ls)) {
		return NULL;
	}
	item = ls->flink;
	removeEnqItem(item);
	item->flink = item;
	item->blink = item;

	return item;
}

int removeEnqItem(enqdlist_t *item)
{
	item->flink->blink = item->blink;
	item->blink->flink = item->flink;

	return true;
}
