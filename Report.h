//
// Created by kev-laptop on 2018-03-10.
//

#ifndef INC_300ASSIGNMENT3_REPORT_H
#define INC_300ASSIGNMENT3_REPORT_H

#include <stdbool.h>

typedef struct REPORT{
    int successFailure;
    char actionIsTaken[];

    bool isSendOrReceive;
    char msgSource[];
    char msg[];

    bool returnsPID;
    int pid;

} REPORT;
#endif //INC_300ASSIGNMENT3_REPORT_H
