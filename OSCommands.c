//
// Created by kev-laptop on 2018-03-13.
//
#include <stdlib.h>
#include "OSCommands.h"
#include "list.h"

static int currentFreeID = 0;
char* queueTextArray[3] = {" High Priority Queue"," Medium Priority Queue"," Low Priority Queue"};
const int NUM_READY_QUEUES = 3;
const int NUM_QUEUES = 4;

//START OF HELPER FUNCTIONS
bool canTerminateInit(){
    if(runningProcess.id != INIT_PROCESS.id){
        return false;
    }

    LIST* listArray[] = {readyQueueLowPriority, readyQueueHighPriority, readyQueueMedPriority, blockedQueue};
    for(int i = 0; i < 4; i++){
        if(ListCount(listArray[i]) != 0){
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
    int* listPid = (int*)item;
    int* searchingPid = (int*)pid;
    return *listPid == *searchingPid;
}

char* returnMallocedChar(char placeholder[]){
    char* msg = malloc(strlen(placeholder)+1);
    memcpy(msg,placeholder,strlen(placeholder)+1);
    msg[strlen(placeholder)] = '\0';
    return msg;
}

REPORT failToTerminateInit(){
    REPORT report;
    report.successFailure = false;

    //char temp[] = "Cannot terminate init process unless no other processes are running or blocked.\0";
    report.actionTaken = "Cannot terminate init process unless no other processes are running or blocked.\0";;
    return report;
}

//END OF HELPER FUNCTIONS

REPORT Create(int priority){
    LIST* listArray[3] = {readyQueueHighPriority, readyQueueMedPriority, readyQueueLowPriority};

    PROCESS* process = malloc(sizeof(PROCESS));
    process->id = currentFreeID;
    process->priority = priority;
    currentFreeID++;

    int result;

    if(runningProcess.id == INIT_PROCESS.id){
        result = true;
        runningProcess = *process;
    }else{
        result = ListPrepend(listArray[priority],process);
    }

    REPORT report;

    if(!result){
        report.successFailure = false;
        report.pid = process->id;
        //char failure[] = "Failed to create process with given priority.\0";
        report.actionTaken = "Failed to create process with given priority.\0";
        return report;
    }else{
        report.successFailure = true;
    }

    char success[] = "Success. New process is created with id ";
    char pidChar[30];
    itoa(process->id,pidChar,10);
    strcat(success,pidChar);
    memcpy(report.actionTaken,success,strlen(success));
    return report;
}

REPORT Fork(){
    if(runningProcess.id == INIT_PROCESS.id){
        REPORT report;
        report.successFailure = false;
        return report;
    }

    REPORT report = Create(runningProcess.priority);
    return report;
}

REPORT Kill(int pid){
    if(pid == INIT_PROCESS.id){
        if(canTerminateInit()){
            terminateSimulation();
        }

        REPORT report = failToTerminateInit();
        return report;
    }

    if(pid == runningProcess.id){
        REPORT report = Exit();
        return report;
    }

    REPORT report;

    int* pidPointer;
    pidPointer = &pid;
    LIST* listArray[] = {readyQueueHighPriority, readyQueueMedPriority, readyQueueLowPriority, blockedQueue};
    bool found = false;

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

    if(!found){
        report.successFailure = false;
        //char failure[] = "Failed to create process with given priority.\0";
        report.actionTaken = "Unable to kill given process.\0";
        return report;
    }else{
        free(ListRemove(listArray[queueWithElement]));
        report.successFailure = true;
        //char failure[] = "Failed to create process with given priority.\0";
        report.actionTaken = "Unable to kill given process.\0";
        return report;
    }

}

REPORT Exit(){
    LIST* listArray[4] = {readyQueueHighPriority, readyQueueMedPriority, readyQueueLowPriority, blockedQueue};

    if(runningProcess.id == INIT_PROCESS.id){
        exit(-1);
    }
}

REPORT Quantum(){

}

REPORT Send(int pid, char msg[]){

}

REPORT Receive(){

}

REPORT Reply(int pid, char msg[]){

}

REPORT NewSemaphore(int semID, int initialValue){

}

REPORT SemaphoreP(int semID){

}

REPORT SemaphoreV(int semID){

}

REPORT Procinfo(int pid){

}

REPORT Totalinfo(){

}