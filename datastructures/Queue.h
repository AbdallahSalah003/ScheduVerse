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
} typedef Queue;

Queue* constructQueue();
bool push(Queue* queue, ProcessData *pData);
struct Node* pop(Queue* queue);
bool empty(Queue* queue);
struct Node* peek(Queue* queue);