//
// Created by kev-laptop on 2018-03-10.
//

#ifndef INC_300ASSIGNMENT3_SEMAPHORE_H
#define INC_300ASSIGNMENT3_SEMAPHORE_H

#include "list.h"
#include <stdbool.h>

typedef struct SEMAPHORE{
    int value;
    bool initialized;
    LIST* listOfWaitingProcesses;
} SEMAPHORE;
#endif //INC_300ASSIGNMENT3_SEMAPHORE_H
