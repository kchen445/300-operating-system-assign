#include <stdio.h>
#include <stdlib.h>
#include "OSCommands.h"
#include "InputHandler.h"

SEMAPHORE sems[5];
PROCESS INIT_PROCESS;
LIST* readyQueueHighPriority;
LIST* readyQueueMedPriority;
LIST* readyQueueLowPriority;
LIST* blockedSendQueue;
LIST* blockedReceiveQueue;
PROCESS* runningProcess;
int currentFreeID;


int main() {
    //initialize global variables
    readyQueueHighPriority = ListCreate();
    readyQueueMedPriority = ListCreate();
    readyQueueLowPriority = ListCreate();
    blockedSendQueue = ListCreate();
    blockedReceiveQueue = ListCreate();
    currentFreeID = 1;

    for(int i = 0; i < 5; i++){
        sems[i].initialized = false;
    }

    INIT_PROCESS.id = 0;
    INIT_PROCESS.priority = 0;
    INIT_PROCESS.otherId = -1;
    INIT_PROCESS.message = NULL;

    runningProcess = &INIT_PROCESS;

    //the actual simulation
    while(true){
        char input = getMenuInput();
        //create
        if(input == 'c' || input == 'C'){
            int priority = getPriority();
            Create(priority);
        }
        //fork
        else if (input == 'f' || input == 'F'){
            Fork();
        }
        //kill
        else if (input == 'k' || input == 'K'){
            int pid = getPid();
            Kill(pid);
        }
        //exit
        else if (input == 'e' || input == 'E'){
            Exit();

        }
        //quantum
        else if (input == 'q' || input == 'Q'){
            Quantum();
        }
        //send
        else if (input == 's' || input == 'S'){
            int pid = getPid();
            char* msg = getProcMsg();
            Send(pid, msg);
        }
        //receive
        else if (input == 'r' || input == 'R'){
            Receive();
        }
        //reply
        else if (input == 'y' || input == 'Y'){
            int pid = getPid();
            char* msg = getProcMsg();
            Reply(pid, msg);

        }
        //new semaphore
        else if (input == 'n' || input == 'N'){
            int semID = getSemID();
            int initialValue = getValue();
            NewSemaphore(semID,initialValue);

        }
        //semaphore P
        else if (input == 'p' || input == 'P'){
            int semID = getSemID();
            SemaphoreP(semID);

        }
        //semaphore V
        else if (input == 'v' || input == 'V'){
            int semID = getSemID();
            SemaphoreV(semID);
        }
        //procinfo
        else if (input == 'i' || input == 'I'){
            int pid = getPid();
            Procinfo(pid);
        }
        //total info
        else if (input == 't' || input == 'T'){
            Totalinfo();
        }else if (input == 'z'){
            exit(0);
        }
        //invalid input
        else{
            printf("Invalid input. Please try again.\n");
        }
    }
}