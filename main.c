#include <stdio.h>
#include <stdlib.h>
#include "OSCommands.h"

PROCESS INIT_PROCESS;
LIST* readyQueueHighPriority;
LIST* readyQueueMedPriority;
LIST* readyQueueLowPriority;
LIST* blockedQueue;
PROCESS runningProcess;

int main() {
    readyQueueHighPriority = ListCreate();
    readyQueueMedPriority = ListCreate();
    readyQueueLowPriority = ListCreate();
    blockedQueue = ListCreate();

    INIT_PROCESS.id = -1;
    INIT_PROCESS.priority = 0;

    runningProcess = INIT_PROCESS;



    Exit();
}