//
// Created by kev-laptop on 2018-03-13.
//
#include <stdlib.h>
#include "OSCommands.h"


char* queueTextArray[] = {"High Priority Queue", "Medium Priority Queue", "Low Priority Queue", "Blocking Send Queue", "Blocking Receive Queue"};
const int NUM_READY_QUEUES = 3;
const int NUM_QUEUES = 5;

//START OF HELPER FUNCTIONS

//checks if init is the last process in the system, after which init can be terminated by exit or kill
bool canTerminateInit(){
    if(runningProcess->id != INIT_PROCESS.id){
        return false;
    }

    LIST* listArray[] = {readyQueueLowPriority, readyQueueHighPriority, readyQueueMedPriority, blockedSendQueue, blockedReceiveQueue};
    for(int i = 0; i < 5; i++){
        if(ListCount(listArray[i]) != 0){
            return false;
        }
    }

    for(int i = 0; i < 5; i++){
        if(!sems[i].initialized){
            continue;
        }
        if(ListCount(sems[i].listOfWaitingProcesses) != 0){
            return false;
        }
    }

    return true;
}

void terminateSimulation(){
    printf("Simulation Terminated.\n");
    exit(0);
}

int idComparator(void* item, void* pid){
    PROCESS* proc = (PROCESS*)item;
    int listPid = proc->id;
    int* searchingPid = (int*)pid;
    return listPid == *searchingPid;
}

//a sending process will store the id of the recipient, this function
//lets ListSearch check for the recipient id in the blocked send queue
int messageIdComparator(void* blockedProc, void* runningPid){
    int messageRecipient = ((PROCESS*)blockedProc)->otherId;
    int* searchingPid = (int*)runningPid;
    return messageRecipient == *searchingPid;
}

void runningProcessHasMessage(){
    printf("Running process received message from process %d:\n %s\n", runningProcess->otherId, runningProcess->message);
    free(runningProcess->message);
    runningProcess->message = NULL;
    runningProcess->otherId = -1;
}

PROCESS* findNextRunningProcess(){
    LIST* listArray[] = {readyQueueHighPriority, readyQueueMedPriority, readyQueueLowPriority};
    PROCESS* newProcess = NULL;
    for(int i = 0; i < NUM_READY_QUEUES; i++){
        if(ListCount(listArray[i]) != 0){
            newProcess = ListTrim(listArray[i]);
            break;
        }
    }
    return newProcess;
}

PROCESS* changeRunningProcess(){
    PROCESS* oldProc = runningProcess;
    PROCESS* newProcess = findNextRunningProcess();
    if(newProcess == NULL){
        runningProcess = &INIT_PROCESS;
        printf("Current process is now the init process.\n");
    }else{
        runningProcess = newProcess;
        printf("Process %d is now the running process.\n", runningProcess->id);
    }
    if (runningProcess->message != NULL){
        runningProcessHasMessage();
    }
    return oldProc;
}

//END OF HELPER FUNCTIONS

void Create(int priority){
    LIST* listArray[3] = {readyQueueHighPriority, readyQueueMedPriority, readyQueueLowPriority};

    PROCESS* process = malloc(sizeof(PROCESS));
    process->id = currentFreeID;
    process->priority = priority;
    process->otherId = -1;
    process->message = NULL;
    //this simulation does not reuse the id's of processes that have left the system
    currentFreeID++;

    //replace init as the running process with the newly created process
    //otherwise add the new process to the appropriate queue
    if(runningProcess->id == INIT_PROCESS.id){
        runningProcess = process;
        printf("Success. Process with ID %d has been created and is now the running process.\n", process->id);
    }else{
        int result = ListPrepend(listArray[priority],process);
        if(result == 0){
            printf("Success. Process with ID %d has been created and added to the %s.\n", process->id,queueTextArray[process->priority]);
        }else{
            printf("Failure. Process was unable to be created.\n");
            free(process);
        }
    }
}

void Fork(){
    //prevent the init function from being forked
    //and simply create a new process with the same priority as the running process
    if(runningProcess->id == INIT_PROCESS.id){
        printf("Failure. Unable to fork initial process.\n");
    }

    Create(runningProcess->priority);
}

void Kill(int pid){
    if(pid == INIT_PROCESS.id){
        if(canTerminateInit()){
            terminateSimulation();
        }
        printf("Unable to terminate initial process.\n");
        return;
    }

    if(pid == runningProcess->id){
        Exit();
        return;
    }

    int* pidPointer;
    pidPointer = &pid;
    LIST* listArray[] = {readyQueueHighPriority, readyQueueMedPriority, readyQueueLowPriority, blockedSendQueue, blockedReceiveQueue};

    bool found = false;
    bool inSems = false;

    //have each of the lists start searching from the first element
    for(int i = 0; i < NUM_QUEUES; i++){
        ListFirst(listArray[i]);
    }

    int queueWithElement = 0;
    for(int i = 0; i < NUM_QUEUES; i++){
        if(ListSearch(listArray[i],idComparator,pidPointer) != NULL){
            found = true;
            queueWithElement = i;
            break;
        }
    }

    //check the waiting lists of the semaphores for the process to kill
    if(!found){
        for(int i = 0; i < 5; i++){
            if(!sems[i].initialized){
                continue;
            }
            ListFirst(sems[i].listOfWaitingProcesses);
            if(ListSearch(sems[i].listOfWaitingProcesses, idComparator, pidPointer) != NULL){
                found = true;
                inSems = true;
                queueWithElement = i;
            }
        }
    }

    if(!found){
        printf("Failure. Unable to locate process with given pid.\n");
        return;
    }

    PROCESS* processToBeRemoved;
    if(!inSems){
        processToBeRemoved = ListRemove(listArray[queueWithElement]);
    }else{
        processToBeRemoved = ListRemove(sems[queueWithElement].listOfWaitingProcesses);
    }

    if(processToBeRemoved == NULL){
        printf("Failure. Process unable to be removed from queue. No action taken.\n");
        return;
    }

    //ready processes can have messages waiting to be printed
    //these messages are stored in the heap and need to be freed before the
    //process is removed
    free(processToBeRemoved->message);
    free(processToBeRemoved);
    if(!inSems){
        printf("Success. Process removed from %s.\n", queueTextArray[queueWithElement]);
    }else{
        printf("Success. Process removed from waiting list of Semaphore %d.\n",queueWithElement);
    }
}

void Exit(){

    if(runningProcess->id == INIT_PROCESS.id){
        if(canTerminateInit()){
            terminateSimulation();
        }
        printf("Failure. Unable to terminate initial process.\n");
        return;
    }

    //changeRunningProcess will have its own print statement that adds to this success string
    printf("Success. ");
    PROCESS* oldProc = changeRunningProcess();
    free(oldProc);
    if (runningProcess->message != NULL){
        runningProcessHasMessage();
    }
}

void Quantum(){
    LIST* listArray[3] = {readyQueueHighPriority, readyQueueMedPriority, readyQueueLowPriority};

    PROCESS* nextProcess = findNextRunningProcess();
    if(nextProcess == NULL){
        printf("No action taken, no other processes to switch to.\n");
        return;
    }

    //prevent quantum from adding the init process to one of the ready queues
    if(runningProcess->id == INIT_PROCESS.id){
        runningProcess = nextProcess;
        printf("Process %d is now the running process instead of the init process.\n", nextProcess->id);
        return;
    }

    int result = ListPrepend(listArray[runningProcess->priority],runningProcess);
    if(result == 0){
        PROCESS* oldProcess = runningProcess;
        runningProcess = nextProcess;
        printf("Process %d is now the running process and process %d has been placed at the back of the %s.\n",
               runningProcess->id, oldProcess->id, queueTextArray[oldProcess->priority]);
        if (runningProcess->message != NULL){
            runningProcessHasMessage();
        }

    }else{
        printf("Error occurred, unable to switch processes.\n");
    }
}

void Send(int pid, char msg[]){

    int* pidPointer = &pid;
    LIST* listArray[3] = {readyQueueHighPriority, readyQueueMedPriority, readyQueueLowPriority};
    bool immediatelyFound = false;
    ListFirst(blockedReceiveQueue);

    //check to see if the recipient of the message is already waiting to receive a message
    //if so then give them the message and unblock them before blocking yourself
    if(ListSearch(blockedReceiveQueue,idComparator, pidPointer) != NULL){
        PROCESS* otherProc = ListRemove(blockedReceiveQueue);
        if(otherProc == NULL){
            free(msg);
            printf("Failure. Receiver was found however process could not be removed from queue, current process is not blocked, message not sent.\n");
            return;
        }
        otherProc->otherId = runningProcess->id;
        otherProc->message = msg;
        int result = ListPrepend(listArray[otherProc->priority], otherProc);
        if(result == 0){
            immediatelyFound = true;
            printf("Message received. Process %d is now placed onto the %s.\n", otherProc->id, queueTextArray[otherProc->priority]);
        }else{
            free(msg);
            printf("Failure. Receiver was found however process could not be added to queue, current process is not blocked, message not sent.\n");
            ListPrepend(blockedReceiveQueue,otherProc);
            return;
        }
    }

    //init can only send a message if the recipient is already blocked
    if(runningProcess->id == INIT_PROCESS.id && immediatelyFound){
        printf("Success. Init process has sent message but remains running process as it cannot block.\n");
        return;
    }else if(runningProcess->id == INIT_PROCESS.id && !immediatelyFound){
        printf("Failure. Recipient not found and init cannot be blocked so this request will not go through.\n");
        return;
    }

    PROCESS* oldProcess = runningProcess;
    int result = ListPrepend(blockedSendQueue,oldProcess);
    if(result == 0){
        if (immediatelyFound){
            oldProcess->otherId = -1;
            oldProcess->message = NULL;
        }else{
            oldProcess->otherId = pid;
            oldProcess->message = msg;
        }
        printf("Success. Process %d has sent a message and is now being blocked. \n", oldProcess->id);
    }else{
        free(msg);
        printf("Failure. Running Process could not send a message.\n");
        return;
    }

    changeRunningProcess();
}

void Receive(){
    bool found = false;
    PROCESS* sendingProc = NULL;

    int* pidPointer = &runningProcess->id;
    ListFirst(blockedSendQueue);

    //again check if the sending process is already waiting to give a message
    //if so then take the message and do not block
    if(ListSearch(blockedSendQueue,messageIdComparator, pidPointer) != NULL){
        found = true;
        sendingProc = ListCurr(blockedSendQueue);
    }

    if(found){
        runningProcess->otherId = sendingProc->id;
        runningProcess->message = sendingProc->message;
        runningProcessHasMessage();
        sendingProc->message = NULL;
        sendingProc->otherId = -1;
    }else{

        if(runningProcess->id == INIT_PROCESS.id){
            printf("No messages for init process. Init cannot be blocked so this request will not go through. \n");
            return;
        }

        PROCESS* oldProc = runningProcess;
        int result = ListPrepend(blockedReceiveQueue, oldProc);
        if(result == 0){
            printf("Process %d is now blocked and waiting for a message.\n", oldProc->id);
        }else{
            printf("Running process is unable to block, no action taken.\n");
            return;
        }
        changeRunningProcess();
    }

}

void Reply(int pid, char msg[]){
    LIST* listArray[3] = {readyQueueHighPriority, readyQueueMedPriority, readyQueueLowPriority};

    int* pidPointer = &pid;
    ListFirst(blockedSendQueue);
    //you cannot reply to a process that is not doing a sending block
    if(ListSearch(blockedSendQueue,idComparator, pidPointer) == NULL){
        free(msg);
        printf("Failure. Process %d is not performing a sending block.\n", pid);
        return;
    }

    PROCESS* proc = ListRemove(blockedSendQueue);
    int result = ListPrepend(listArray[proc->priority],proc);
    if(result == 0){
        //you can reply to a process that has not delivered its message yet however the message will be destroyed
        if(proc->message != NULL){
            free(proc->message);
            printf("Send has a message that has yet to be claimed, but it will be destroyed to unblock process.\n");
        }

        proc->otherId = runningProcess->id;
        proc->message = msg;

        printf("Success. Process %d is unblocked and added to the %s.\n", proc->id, queueTextArray[proc->priority]);
    }else{
        free(msg);
        printf("Failure. Unable to return sending process to a ready queue. Process %d added back to blocking queue.\n",
               proc->id);
        ListPrepend(blockedSendQueue,proc);
    }
}

void NewSemaphore(int semID, int initialValue){
    if(sems[semID].initialized){
        printf("Error. This semaphore has already been initialized. \n");
        return;
    }

    sems[semID].initialized = true;
    sems[semID].listOfWaitingProcesses = ListCreate();
    sems[semID].value = initialValue;
    printf("Success. The semaphore with id %d has been initialized with value %d.\n", semID, initialValue);

}

void SemaphoreP(int semID){
    if(!sems[semID].initialized){
        printf("Failure. Semaphore is not initialized.\n");
        return;
    }
    SEMAPHORE* semaphore = &sems[semID];
    LIST* semList = semaphore->listOfWaitingProcesses;

    //follows implementation in the beginning of lecture 8 where semaphore blocks if less than or equal to 0
    if(semaphore->value <= 0){
        //init can gain control of semaphore but it cannot be placed on the block list
        if(runningProcess->id == INIT_PROCESS.id){
            printf("Failure. Init process cannot gain control over semaphore %d and cannot be blocked. Request "
                           "terminated.\n", semID);
            return;
        }

        PROCESS* oldProc = runningProcess;
        int result = ListPrepend(semList, oldProc);
        if(result == 0){
            printf("Success. Process is now blocked waiting for semaphore %d.\n", semID);
            changeRunningProcess();
        }else{
            printf("Failure. Process could not gain semaphore or be added to block list. No action taken.\n");
            return;
        }

    }else{
        semaphore->value--;
        printf("Success. Process %d now has control over semaphore %d.\n", runningProcess->id, semID);
    }
}

void SemaphoreV(int semID){
    if(!sems[semID].initialized){
        printf("Failure. Semaphore is not initialized.\n");
        return;
    }

    LIST* listArray[3] = {readyQueueHighPriority, readyQueueMedPriority, readyQueueLowPriority};
    SEMAPHORE* semaphore = &sems[semID];
    LIST* semList = semaphore->listOfWaitingProcesses;

    //unblock a waiting process if there is one
    if(ListCount(semList) != 0){
        PROCESS* proc = ListTrim(semList);
        int result = ListPrepend(listArray[proc->priority], proc);
        if(result == 0){
            printf("Process %d is unblocked and placed in the %s.\n", proc->id, queueTextArray[proc->priority]);
        }else{
            printf("Failure. Unable to unblock process waiting on semaphore. No action taken.\n");
            ListAppend(semList, proc);
            return;
        }
    }

    //no blocks to calling V therefore the semaphore can have a value greater than its initial value
    semaphore->value++;
    printf("Success. Semaphore %d has been incremented.\n", semID);
}

void Procinfo(int pid){

    //the function will return where a process is placed (running, ready queue, blocked)
    //the id, priority, and whether or not a process has a message waiting to be sent or printed to the screen
    int* pidPointer = &pid;

    char* blockedQueueText[] = {"Blocking Send Queue", "Blocking Receive Queue"};
    LIST* readyQueues[3] = {readyQueueHighPriority,readyQueueMedPriority,readyQueueLowPriority};
    LIST* blockedQueues[2] = {blockedSendQueue,blockedReceiveQueue};

    if(pid == INIT_PROCESS.id){
        printf("Process is the init process.\n");
        if(runningProcess->id == INIT_PROCESS.id){
            printf("The init process is currently running.\n");
        }else{
            printf("The init process is not currently running.\n");
        }
        return;
    }

    if(runningProcess->id == pid){
        printf("Process is currently running.\n");
        printf("ID: %d\n", runningProcess->id);
        printf("Priority: %d\n", runningProcess->priority);
        printf("Process is not sending or waiting for a message.\n");
        return;
    }

    for(int i = 0; i < 3; i++){
        ListFirst(readyQueues[i]);
        if(ListSearch(readyQueues[i],idComparator,pidPointer) != NULL){
            PROCESS* p = ListCurr(readyQueues[i]);
            printf("Process is waiting to execute in the %s.\n", queueTextArray[i]);
            printf("ID: %d\n", p->id);
            printf("Priority: %d\n", p->priority);
            if(p->message != NULL){
                printf("Process has been replied to by another process with the message:\n%s \n",p->message);
            }else{
                printf("Process is not sending or waiting for a message.\n");
            }
            return;
        }
    }

    for(int i = 0; i < 2; i++){
        ListFirst(blockedQueues[i]);
        if(ListSearch(blockedQueues[i],idComparator,pidPointer) != NULL){
            PROCESS* p = ListCurr(blockedQueues[i]);
            printf("Process is being blocked in the %s.\n", blockedQueueText[i]);
            printf("ID: %d\n", p->id);
            printf("Priority: %d\n", p->priority);
            if(i == 0){
                if(p->message != NULL){
                    printf("Process is sending a message to process %d:\n%s\n", p->otherId, p->message);
                }else{
                    printf("Process has sent a message and is waiting to be unblocked.\n");
                }
            }else{
                printf("Process is waiting to receive a message.\n");
            }
            return;
        }
    }

    for(int i = 0; i < 5; i++) {
        ListFirst(sems[i].listOfWaitingProcesses);
        if (ListSearch(sems[i].listOfWaitingProcesses, idComparator, pidPointer) != NULL) {
            PROCESS *p = ListCurr(sems[i].listOfWaitingProcesses);
            printf("Process is being blocked waiting for semaphore %d.\n", i);
            printf("ID: %d\n", p->id);
            printf("Priority: %d\n", p->priority);
            if (p->message != NULL) {
                printf("Process has been replied to by another process with the message:\n%s \n", p->message);
            } else {
                printf("Process is not sending or waiting for a message.\n");
                return;
            }
        }
    }
    printf("Process with given pid could not be found.\n");

}

void Totalinfo(){
    //this function lists out what the running process is and the ids of every process in each queue

    LIST* listArray[] = {readyQueueHighPriority, readyQueueMedPriority, readyQueueLowPriority, blockedSendQueue, blockedReceiveQueue};
    printf("Process %d is the running process.\n", runningProcess->id);

    for(int i = 0; i < 5; i++){
        if(ListCount(listArray[i]) == 0){
            printf("No processes are on the %s.\n", queueTextArray[i]);
            continue;
        }else{
            ListFirst(listArray[i]);
            printf("%s holds the following process(es):\n", queueTextArray[i]);
            while(true){
                PROCESS* curr = ListCurr(listArray[i]);
                PROCESS* next = ListNext(listArray[i]);
                if(next == NULL){
                    printf("%d\n",curr->id);
                    break;
                }else{
                    printf("%d, ", curr->id);
                }
            }
        }
    }

    for(int i = 0; i < 5; i++){
        if(!sems[i].initialized){
            printf("Semaphore %d has not been initialized.\n", i);
            continue;
        }
        printf("Semaphore %d has value %d.\n", i, sems[i].value);
        if(ListCount(sems[i].listOfWaitingProcesses) == 0){
            printf("No process are blocked waiting for this semaphore.\n");
        }else{
            ListFirst(sems[i].listOfWaitingProcesses);
            printf("Semaphore %d is blocking the following processes:\n", i);
            while(true){
                PROCESS* curr = ListCurr(sems[i].listOfWaitingProcesses);
                PROCESS* next = ListNext(sems[i].listOfWaitingProcesses);
                if(next == NULL){
                    printf("%d\n",curr->id);
                    break;
                }else{
                    printf("%d, ", curr->id);
                }
            }
        }

    }
}