#include <stdio.h>
#include <stdlib.h>
#include "OSCommands.h"

const int INIT_PROCESS = -1;
LIST* readyQueue;
LIST* blockedQueue;
int runningProcess;

int main() {
    readyQueue = ListCreate();
    blockedQueue = ListCreate();
    runningProcess = INIT_PROCESS;
    Exit();
}