#ifndef LIST_STRUCT
#define LIST_STRUCT
#include <stdio.h>
#include "LinkedList.h"

LIST* ListCreate();
int ListCount(LIST* list);
void* ListFirst(LIST* list);
void* ListLast(LIST* list);
void* ListNext(LIST* list);
void* ListPrev(LIST* list);
void* ListCurr(LIST* list);
int ListAdd(LIST* list, void* item);
int ListInsert(LIST* list, void* item);
int ListAppend(LIST* list, void* item);
int ListPrepend(LIST* list, void* item);
void* ListRemove(LIST* list);
void ListConcat(LIST* list1, LIST* list2);
void ListFree(LIST* list, void (*itemFree)(void*));
void* ListTrim(LIST* list);
void* ListSearch(LIST* list, int (*comparator)(void*, void*), void* comparisonArg);

#endif