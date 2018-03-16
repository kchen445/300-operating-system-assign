//
// Created by kev-laptop on 2018-03-13.
//

#include "InputHandler.h"

bool stringIsIntAndInRange (char* string, int lowerBound, int upperBound, int stringSize, int maxDigits){
    if(stringSize > maxDigits){
        return false;
    }

    for(int i = 0; i < stringSize; i++){
        if(string[i] < '0' || string[i] > upperBound + '0'){
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
        bytesRead = read(STDIN_FILENO,buf,50);
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
    char buf[50];
    int num = -1;
    while(num != 1){
        num = read(STDIN_FILENO,buf,50);
        if(num != 1){
            printf("Too many characters entered. Try again!\n");
        }
    }
    return buf[0];
}

int getPriority(){
    return getAIntInputInRange(0,2);
}

int getPid(){
    int upperBound = currentFreeID - 1;
    if(currentFreeID == 0){
        upperBound = 0;
    }
    return getAIntInputInRange(0, upperBound);
}

char* getProcMsg(){
    char buf[500];
    int bytesRead = 0;
    bool valid = false;
    while(!valid){
        bytesRead = read(STDIN_FILENO,buf,40);
        if(bytesRead > 40){
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
    return getAIntInputInRange(0,4);
}

int getValue(){
    return getAIntInputInRange(INT_MIN,INT_MAX);
}