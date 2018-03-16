//
// Created by kev-laptop on 2018-03-17.
//

#ifndef INC_300ASSIGNMENT3_SHAREDDATA_H
#define INC_300ASSIGNMENT3_SHAREDDATA_H
#include "list.h"
#include "Process.h"
#include "Semaphore.h"

extern SEMAPHORE sems[4];
extern PROCESS INIT_PROCESS;
extern PROCESS* runningProcess;
extern LIST* readyQueueHighPriority;
extern LIST* readyQueueMedPriority;
extern LIST* readyQueueLowPriority;
extern LIST* blockedSendQueue;
extern LIST* blockedReceiveQueue;
extern int currentFreeID;
#endif //INC_300ASSIGNMENT3_SHAREDDATA_H
