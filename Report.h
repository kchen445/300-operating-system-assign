//
// Created by kev-laptop on 2018-03-10.
//

#ifndef INC_300ASSIGNMENT3_REPORT_H
#define INC_300ASSIGNMENT3_REPORT_H

#include <stdbool.h>
#include "Semaphore.h"

typedef struct REPORT{
    int successFailure;

    char* actionTaken;

    int msgSource;
    char* msg;

    int pid;

} REPORT;
#endif //INC_300ASSIGNMENT3_REPORT_H
