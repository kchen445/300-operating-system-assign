//
// Created by kev-laptop on 2018-03-13.
//

#include "InputHandler.h"

bool stringIsIntAndInRange (char* string, int lowerBound, int upperBound, int stringSize, int maxDigits){
    if(string[0] != '-'){
        if(stringSize > maxDigits){
            return false;
        }
    }else{
        if(stringSize - 1 > maxDigits){
            return false;
        }
    }

    //ensures a maximum of 999,999,999 to prevent undefined behaviour from atoi when an int overflows
    if(maxDigits > 9){
        return false;
    }

    for(int i = 0; i < stringSize; i++){
        if(i == 0 && string[i] == '-'){
            continue;
        }
        if(string[i] < '0' || string[i] > '9'){
            return false;
        }
    }

    int num = atoi(string);

    if(num < lowerBound || num > upperBound){
        return false;
    }

    return true;
}

int findNumberOfDigitsInNumber(int number){
    int counter = 0;
    if (number == 0){
        counter++;
    }
    while(number != 0){
        number /= 10;
        counter++;
    }
    return counter;
}

int getAIntInputInRange(int lowerBound, int upperBound){
    char buf[50];
    int bytesRead = 0;
    int maxNumberOfDigits = findNumberOfDigitsInNumber(upperBound);
    bool valid = false;
    while(!valid){
        //use read to get input
        bytesRead = read(STDIN_FILENO,buf,50);
        bytesRead = bytesRead -1;

        char* pos;
        if((pos=strchr(buf,'\n')) != NULL){
            *pos = '\0';
        }

        if(!stringIsIntAndInRange(buf,lowerBound,upperBound,bytesRead,maxNumberOfDigits)){
            printf("Invalid input, try again.\n");
            continue;
        }

        valid = true;
    }

    int outputInt = atoi(buf);
    return outputInt;
}

char getMenuInput(){
    printf("Please enter a character for the menu: \n");
    char buf[50];
    int num = -1;
    while(num != 2){
        num = read(STDIN_FILENO,buf,50);
        if(num != 2){
            printf("Too many characters entered. Try again!\n");
        }
    }
    return buf[0];
}

int getPriority(){
    printf("Please enter an integer from 0 to 2 for the process priority: \n");
    return getAIntInputInRange(0,2);
}

int getPid(){
    int upperBound = currentFreeID - 1;
    if(currentFreeID == 1){
        upperBound = 0;
    }
    printf("Please enter the pid of desired process, values past %d have not been assigned: \n", upperBound);

    return getAIntInputInRange(0, upperBound);
}

char* getProcMsg(){
    printf("Please enter the message for the process: \n");
    char buf[500];
    int bytesRead = 0;
    bool valid = false;
    while(!valid){
        bytesRead = read(STDIN_FILENO,buf,39);

        bytesRead--;
        char* pos;
        if((pos=strchr(buf,'\n')) != NULL){
            *pos = '\0';
        }

        if(bytesRead > 39){
            printf("Too many characters entered. Try again!");
            continue;
        }
        if(bytesRead < 0){
            printf("Error occurred, please try again.");
            continue;
        }
        valid = true;
    }
    buf[bytesRead] = '\0';
    char* msg = malloc(bytesRead+1);
    memcpy(msg,buf,bytesRead+1);
    return msg;
}

int getSemID(){
    printf("Please enter a semaphore ID: \n");
    return getAIntInputInRange(0,4);
}

int getValue(){
    printf("Please enter 0 or a positive value less than 1 billion: \n");
    const int UNDER_BILLION = 999999999;
    return getAIntInputInRange(0,UNDER_BILLION);
}