#include "list.h"

#define MAX_NUM_NODES 500
#define MAX_NUM_HEADS 20
static NODE nodeArray[MAX_NUM_NODES];
static LIST headArray[MAX_NUM_HEADS];
static NODE* nextAvailableNode;
static LIST* nextAvailableHead;

//START OF HELPER FUNCTIONS
bool firstRun = true;

void addItemToEmptyList(LIST* list, void* item){

    NODE* newNode = nextAvailableNode;
    nextAvailableNode = nextAvailableNode->nextNode;
    newNode->item = item;

    newNode->nextNode = NULL;
    newNode->prevNode = NULL;

    list->current = newNode;
    list->first = newNode;
    list->last = newNode;
    list->currentItem = list->current->item;

    list->itemCount++;
}

void initializePools(){
    //have unused nodes' next node point to next available node
    //have unused heads' item member point to the next available head
    for(int i = 0; i < MAX_NUM_NODES - 1; i++){
        nodeArray[i].nextNode = &nodeArray[i+1];
    }
    nodeArray[MAX_NUM_NODES-1].nextNode = NULL;
    nextAvailableNode = &nodeArray[0];

    for(int i = 0; i < MAX_NUM_HEADS - 1; i++){
        headArray[i].currentItem = &headArray[i+1];
    }
    nextAvailableHead = &headArray[0];
    headArray[MAX_NUM_HEADS-1].currentItem = NULL;
}

void freeNode(NODE* nodeToBeFreed){
    nodeToBeFreed->nextNode = nextAvailableNode;
    nodeToBeFreed->prevNode = NULL;
    nodeToBeFreed->item = NULL;
    nextAvailableNode = nodeToBeFreed;
}

void freeList(LIST* list){
    list->last = NULL;
    list->first = NULL;
    list->current = NULL;
    list->currentItem = nextAvailableHead;
    list->itemCount = 0;
    list->currentIsBehindFirst = false;
    list->currentIsAheadOfLast = false;
    nextAvailableHead = list;
}

bool hasAvailableNode(){
    return nextAvailableNode != NULL;
}

bool hasAvailableHead(){
    return nextAvailableHead != NULL;
}

bool validList(LIST *list){
    return list != NULL;
}

int addingNodeCommonCases(LIST *list, void *item, bool isPrependOrAppend){
    if(!hasAvailableNode()||!validList(list)){
        return -1;
    }

    if(list->itemCount == 0){
        addItemToEmptyList(list,item);
        return 0;
    }
    if(!isPrependOrAppend){
        if(list->currentIsAheadOfLast){
            return ListAppend(list,item);
        }
        if(list->currentIsBehindFirst){
            return ListPrepend(list,item);
        }
    }

    int noCasesApply = -5;
    return noCasesApply;

}


//END OF HELPER FUNCTIONS

LIST* ListCreate(){
    if(firstRun){
        firstRun = false;
        initializePools();
    }
    if(hasAvailableHead()){
		LIST* newHead = nextAvailableHead;
        newHead->itemCount = 0;
        newHead->currentIsAheadOfLast = false;
        newHead->currentIsBehindFirst = false;
        nextAvailableHead = nextAvailableHead->currentItem;
        return newHead;
    }else{
        return NULL;
    }
}

int ListCount(LIST* list){
    if(!validList(list)){
        return 0;
    }
    return list->itemCount;
}

void* ListFirst(LIST* list){
    if(!validList(list)||list->itemCount == 0){
        return NULL;
    }

    list->current = list->first;
    list->currentItem = list->current->item;
    list->currentIsAheadOfLast = false;
    list->currentIsBehindFirst = false;
    return list->currentItem;
}

void* ListLast(LIST* list){
    if(!validList(list)||list->itemCount == 0){
        return NULL;
    }

    list->current = list->last;
    list->currentItem = list->current->item;
    list->currentIsAheadOfLast = false;
    list->currentIsBehindFirst = false;
    return list->currentItem;
}

void* ListNext(LIST* list){
    if(!validList(list)||list->itemCount == 0){
        return NULL;
    }
    if(list->current == list->last){
        list->currentIsAheadOfLast = true;
        return NULL;
    }else if(list->currentIsBehindFirst){

        list->current = list->first;
        list->currentIsBehindFirst = false;

    }else{
        list->current = list->current->nextNode;
    }
    list->currentItem = list->current->item;
    return list->currentItem;
}

void* ListPrev(LIST* list){
    if(!validList(list)||list->itemCount == 0){
        return NULL;
    }
    if(list->current == list->first){
        list->currentIsBehindFirst = true;
        return NULL;
    }else if(list->currentIsAheadOfLast){

        list->current = list->last;
        list->currentIsAheadOfLast = false;

    }else{
        list->current = list->current->prevNode;
    }
    list->currentItem = list->current->item;
    return list->currentItem;
}

void* ListCurr(LIST* list){
    if(!validList(list)||
       (list->currentIsAheadOfLast || list->currentIsBehindFirst)||
       (list->itemCount == 0)) {
        return NULL;
    }
    return list->currentItem;
}

int ListAdd(LIST* list, void* item){
    bool isPrependOrAppend = false;
    int specialCase = addingNodeCommonCases(list,item,isPrependOrAppend);
    if(specialCase == -1 || specialCase == 0){
        return specialCase;
    }

    if(list->current == list->last) {
        return ListAppend(list, item);
    }

    NODE* newNode = nextAvailableNode;
    nextAvailableNode = nextAvailableNode->nextNode;
    newNode->item = item;

    NODE* nodeAfterCurrent = list->current->nextNode;

    list->current->nextNode = newNode;
    nodeAfterCurrent->prevNode = newNode;
    newNode->prevNode = list->current;
    newNode->nextNode = nodeAfterCurrent;

    list->current = newNode;
    list->currentItem = list->current->item;
    list->itemCount++;
    return 0;
}

int ListInsert(LIST* list, void* item){
    bool isPrependOrAppend = false;
    int specialCase = addingNodeCommonCases(list,item,isPrependOrAppend);
    if(specialCase == -1 || specialCase == 0){
        return specialCase;
    }

    if(list->current == list->first){
        return ListPrepend(list,item);
    }

    NODE* newNode = nextAvailableNode;
    nextAvailableNode = nextAvailableNode->nextNode;
    newNode->item = item;

    NODE* nodeBeforeCurrent = list->current->prevNode;

    list->current->prevNode = newNode;
    nodeBeforeCurrent->nextNode = newNode;
    newNode->nextNode = list->current;
    newNode->prevNode = nodeBeforeCurrent;

    list->current = newNode;
    list->currentItem = list->current->item;
    list->itemCount++;
    return 0;
}

int ListAppend(LIST* list, void* item){
    bool isPrependOrAppend = true;
    int specialCase = addingNodeCommonCases(list,item,isPrependOrAppend);
    if(specialCase == -1 || specialCase == 0){
        return specialCase;
    }

    NODE* newLast = nextAvailableNode;
    nextAvailableNode = nextAvailableNode->nextNode;
    newLast->item = item;
    newLast->nextNode = NULL;

    newLast->prevNode = list->last;
    list->last->nextNode = newLast;
    list->last = newLast;
    list->current = list->last;
    list->currentIsBehindFirst = false;
    list->currentIsAheadOfLast = false;
    list->currentItem = list->current->item;

    list->itemCount++;
    return 0;
}

int ListPrepend(LIST* list, void* item){
    bool isPrependOrAppend = true;
    int specialCase = addingNodeCommonCases(list,item,isPrependOrAppend);
    if(specialCase == -1 || specialCase == 0){
        return specialCase;
    }

    NODE* newFirst = nextAvailableNode;
    nextAvailableNode = nextAvailableNode->nextNode;
    newFirst->item = item;
    newFirst->prevNode = NULL;

    newFirst->nextNode = list->first;
    list->first->prevNode = newFirst;
    list->first = newFirst;
    list->current = list->first;
    list->currentIsBehindFirst = false;
    list->currentIsAheadOfLast = false;
    list->currentItem = list->current->item;

    list->itemCount++;
    return 0;
}

void* ListRemove(LIST* list){
	if(!validList(list)||
       (list->currentIsAheadOfLast || list->currentIsBehindFirst)||
       (list->itemCount == 0)) {
		return NULL;
	}

	if(list->itemCount == 1){
        return ListTrim(list);
    }

    if(list->current == list->last){
        return ListTrim(list);
    }

    NODE* nodeToBeRemoved = list->current;
    NODE* newCurrent = list->current->nextNode;
    if(nodeToBeRemoved != list->first){
        nodeToBeRemoved->prevNode->nextNode = newCurrent;
    }else{
        list->first=newCurrent;
    }
    newCurrent->prevNode = nodeToBeRemoved->prevNode;
    list->current = newCurrent;
    nodeToBeRemoved->nextNode = NULL;
    nodeToBeRemoved->prevNode = NULL;
    list->currentItem = list->current->item;
    list->itemCount--;

    return nodeToBeRemoved->item;
}

void ListConcat(LIST* list1, LIST* list2){
	if(!validList(list1) && !validList(list2)){
		return;
	}
	
	if(list1->itemCount == 0){
		list1->itemCount = list2->itemCount;
        list1->last = list2->last;
        list1->first = list2->first;
        list1->current = list2->current;
        list1->currentItem = list2->currentItem;
        list1->currentIsBehindFirst = list2->currentIsBehindFirst;
        list1->currentIsAheadOfLast = list2->currentIsAheadOfLast;
        freeList(list2);
		return;
	}
	
	if(list2->itemCount == 0){
        freeList(list2);
        return;
	}

    if(list1==list2){
        return;
    }

	list1->last->nextNode = list2->first;
	list2->first->prevNode = list1->last;
	list1->itemCount += list2->itemCount;
	list1->last = list2->last;
    freeList(list2);
}

void ListFree(LIST* list, void (*itemFree)(void*)){
    if(!validList(list)) {
        return;
    }

    list->current = list->first;
    for(int i = 0; i < list->itemCount;i++){
        void* itemToBeFreed = list->current->item;
        (*itemFree)(itemToBeFreed);
        NODE* nodeToBeFreed = list->current;
        list->current = list->current->nextNode;
        freeNode(nodeToBeFreed);
    }

    freeList(list);
}

void* ListTrim(LIST* list){

    if(list->itemCount == 1){
        ListFirst(list);
        list->first = NULL;
        list->last = NULL;
        void* item = list->current->item;
        freeNode(list->current);
        list->current = NULL;
        list->itemCount--;
        return item;
    }

    NODE* nodeToBeRemoved = list->last;
    list->last = list->last->prevNode;
    list->last->nextNode = NULL;
    list->current = list->last;
    nodeToBeRemoved->prevNode = NULL;
    void* item = nodeToBeRemoved->item;
    freeNode(nodeToBeRemoved);
    list->itemCount--;
    return item;
}

void* ListSearch(LIST* list, int (*comparator)(void*, void*), void* comparisonArg){
    if(!validList(list)||
       (list->currentIsAheadOfLast || list->currentIsBehindFirst)||
       (list->itemCount == 0)) {
        return NULL;
    }

    int found = comparator(ListCurr(list),comparisonArg);
    while(!found){
        void* item = ListNext(list);
        if(item == NULL){
            break;
        }
        found = comparator(item,comparisonArg);
    }
    return ListCurr(list);
}

