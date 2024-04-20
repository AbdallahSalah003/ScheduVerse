#pragma once
#include <stdbool.h>
#include <stdlib.h>

#include "../processData.h"

struct Node
{
    ProcessData pData;
    struct Node *next;
    
};

struct 
{
    struct Node *front;
    struct Node *back;
}typedef priQueue ;

priQueue * constructPriortyQueue();
bool pushSRTN(priQueue* pqueue, ProcessData *pData);
bool pushHPF(priQueue* pqueue, ProcessData *pData);
struct Node* pop(priQueue* queue);
bool empty(priQueue* queue);
struct Node * peek(priQueue* queue);