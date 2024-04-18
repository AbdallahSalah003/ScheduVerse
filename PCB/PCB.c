#include "PCB.h"

PCB constructPCB(ProcessData pData)
{
    PCB newPCB;
    newPCB.pId = pData.id;
    newPCB.startTime = -1;
    newPCB.finishTime = -1;
    newPCB.remainTime = pData.runningtime;
    newPCB.waitTime = 0;
    newPCB.turnaroundTime = 0;
    newPCB.weightedTurnaroundTime = 0;

    newPCB.processData = pData;
    newPCB.state = 1; // READY 
    
    return newPCB;
}
