#pragma once

#include "../process-data/processData.h"

struct 
{
    ProcessData pData;
    Node *next;
    
} typedef Node;
struct
{
    Node *front;
    Node *back;
} typedef Queue;

Queue* constructQueue();
bool push(Queue* queue, ProcessData pData);
Node* pop(Queue* queue);
bool empty(Queue* queue);
Node* peek(Queue* queue);
