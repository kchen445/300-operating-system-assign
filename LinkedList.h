#include "ListNode.h"
#include <stdbool.h>

typedef struct LIST{
    NODE* last;
    NODE* first;
    NODE* current;
    void* currentItem;
    int itemCount;
    bool currentIsBehindFirst;
    bool currentIsAheadOfLast;
} LIST;