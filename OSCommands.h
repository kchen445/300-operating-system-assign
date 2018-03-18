//
// Created by kev-laptop on 2018-03-10.
//
#ifndef INC_300ASSIGNMENT3_OSCOMMANDS_H
#define INC_300ASSIGNMENT3_OSCOMMANDS_H
#include "SharedData.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

void Create(int priority);
void Fork();
void Kill(int pid);
void Exit();
void Quantum();
void Send(int pid, char msg[]);
void Receive();
void Reply(int pid, char msg[]);
void NewSemaphore(int semID, int initialValue);
void SemaphoreP(int semID);
void SemaphoreV(int semID);
void Procinfo(int pid);
void Totalinfo();

#endif //INC_300ASSIGNMENT3_OSCOMMANDS_H
