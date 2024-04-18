#pragma once

typedef struct
{
    int arrivaltime;
    int priority;
    int runningtime;
    int id;
} ProcessData;

ProcessData *constructProcess(int id, int arrive, int run, int prio)
{
    ProcessData *tmp = (struct ProcessData*)malloc(sizeof(ProcessData));
    tmp->id = id;
    tmp->arrivaltime = arrive;
    tmp->runningtime= run;
    tmp->priority = prio;
    return tmp;
}