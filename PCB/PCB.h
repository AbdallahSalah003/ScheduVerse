#pragma once
#include "../processData.h"
#include "globalEnums.h"

struct 
{
    int pId;
    int startTime;
    int finishTime;
    int remainTime;
    int waitTime;
    int turnaroundTime;
    int weightedTurnaroundTime;
    ProcessData processData;
    State state;
} typedef PCB;

PCB constructPCB(ProcessData pdata);
