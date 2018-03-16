#include <stdio.h>
#include <stdlib.h>
#include "OSCommands.h"
#include "InputHandler.h"

SEMAPHORE sems[4];
PROCESS INIT_PROCESS;
LIST* readyQueueHighPriority;
LIST* readyQueueMedPriority;
LIST* readyQueueLowPriority;
LIST* blockedSendQueue;
LIST* blockedReceiveQueue;
PROCESS* runningProcess;
int currentFreeID;

int main() {
    readyQueueHighPriority = ListCreate();
    readyQueueMedPriority = ListCreate();
    readyQueueLowPriority = ListCreate();
    blockedSendQueue = ListCreate();
    blockedReceiveQueue = ListCreate();
    currentFreeID = 0;

    INIT_PROCESS.id = -1;
    INIT_PROCESS.priority = 0;

    runningProcess = &INIT_PROCESS;
    REPORT report;
    while(true){
        char input = getMenuInput();
        int priority;
        int pid;
        char* msg;
        int semID;
        int semVal;
        switch(input){
            //create
            case 'c':
            case 'C':
                priority = getPriority();
                report = Create(priority);
                if(report.successFailure){
                    printf("");
                    printf("%d", report.pid);
                }else{
                    printf("");
                }
                break;

            //fork
            case 'f':
            case 'F':
                report = Fork();
                break;

            //kill
            case 'k':
            case 'K':
                pid = getPid();
                report = Kill(pid);
                break;

            //exit
            case 'e':
            case 'E':
                report = Exit();
                exit(-1);
                break;

            //quantum
            case 'q':
            case 'Q':
                report = Quantum();
                break;

            //send
            case 's':
            case 'S':
                pid = getPid();
                msg = getProcMsg();
                report = Send(pid, msg);
                break;

            //receive
            case 'r':
            case 'R':
                report = Receive();
                break;

            //reply
            case 'y':
            case 'Y':
                pid = getPid();
                msg = getProcMsg();
                report = Reply(pid, msg);
                break;

            //new semaphore
            case 'n':
            case 'N':
                semID = getSemID();
                semVal = getValue();
                report = NewSemaphore(semID, semVal);
                break;

            //semaphore p
            case 'p':
            case 'P':
                semID = getSemID();
                report = SemaphoreP(semID);
                break;

            //semaphore v
            case 'v':
            case 'V':
                semID = getSemID();
                report = SemaphoreV(semID);
                break;
            //process info
            case 'i':
            case 'I':
                pid = getPid();
                report = Procinfo(pid);
                break;
            //total info
            case 't':
            case 'T':
                report = Totalinfo();
                break;
            default:
                printf("Invalid input. Please try again.\n");
        }
    }
}