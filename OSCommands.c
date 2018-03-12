//
// Created by kev-laptop on 2018-03-13.
//
#include <stdlib.h>
#include "OSCommands.h"
#include "list.h"




REPORT Create(int priority){

}

REPORT Fork(){

}

REPORT Kill(int pid){

}

void Exit(){
    if(runningProcess == INIT_PROCESS){
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