#pragma once 

#include <stdbool.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

#include "../process-data/processData.h"

int globalMsgQueueID;

struct 
{
    long mtype;
    ProcessData process;
} typedef MsgBuff;

void constructMsgQueue();
void destroyMsgQueue();
MsgBuff constructMsg(ProcessData process);
bool sendMsg(MsgBuff msg);
bool recvMsg(long msgType);
bool isMsgQueueEmpty();