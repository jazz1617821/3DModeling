#ifndef _ENQLIST_H
#define _ENQLIST_H

// EnqueuableDoubleList 
typedef struct EnqDList
{
	EnqDList *flink;
	EnqDList *blink;
}enqdlist_t;

int initEnqDList(enqdlist_t *item);
int isEnqed(enqdlist_t *item);
int isEnqEmpty(enqdlist_t *ls);
int addEnqHead(enqdlist_t *ls, enqdlist_t *item);
int addEnqTail(enqdlist_t *ls, enqdlist_t *item);
int addEnqBefore(enqdlist_t *target, enqdlist_t *item);
int addEnqAfter(enqdlist_t *target, enqdlist_t *item);
enqdlist_t *removeEnqHead(enqdlist_t *ls);
enqdlist_t *removeEnqTail(enqdlist_t *ls);
int removeEnqItem(enqdlist_t *item);


#endif // ENQLIST_H
