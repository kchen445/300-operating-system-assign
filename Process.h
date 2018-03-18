//
// Created by Kevin on 16/03/2018.
//

#ifndef INC_300ASSIGNMENT3_PROCESS_H
#define INC_300ASSIGNMENT3_PROCESS_H

//a process does not store it's state as it can be easily
//extrapolated from its location in the system
//i.e ready if its in ready queue, blocked if its in the blocked queue
typedef struct PROCESS{
    int id;
    int priority;
    int otherId;
    char* message;
} PROCESS;
#endif //INC_300ASSIGNMENT3_PROCESS_H
