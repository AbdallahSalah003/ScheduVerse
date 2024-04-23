#pragma once
#include "globalEnums.h"

typedef struct
{
    int arrivaltime;
    int priority;
    int runningtime;
    int id;
    int remainingTime;
    int startTime;
    int getCPUBefore;
    int realID;
    State state;
} ProcessData;

ProcessData *constructProcess(int id, int arrive, int run, int prio)
{
    ProcessData *tmp = (struct ProcessData*)malloc(sizeof(ProcessData));
    tmp->id = id;
    tmp->arrivaltime = arrive;
    tmp->runningtime= run;
    tmp->priority = prio;
    tmp->remainingTime = run;
    tmp->getCPUBefore = 0;
    return tmp;
}