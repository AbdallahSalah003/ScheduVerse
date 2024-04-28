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

Queue* constructQueue() 
{
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->back = NULL;
    queue->front = NULL;
    return queue;
}
bool push(Queue* queue, ProcessData* pData)
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->pData = *pData;
    newNode->next = NULL;
    if(queue->back == NULL) 
    {
        queue->front = newNode;
        queue->back = newNode;
    }
    else 
    {
        queue->back->next = newNode; // Add new node
        queue->back = newNode; // update back ptr
    }
    return 1;
}
struct Node* pop(Queue* queue) 
{
    if(queue->front == NULL)
    {
        return NULL;
    }
    struct Node* node = queue->front;
    queue->front = queue->front->next;
    if(queue->front == NULL) 
        queue->back = NULL;
    return node;
}
bool empty(Queue *queue) 
{
    if(queue->front == NULL) 
        return 1;
    return  0;
}
struct Node* peek(Queue *queue)
{
    if(queue->front == NULL) 
        return NULL;
    return  queue->front;
}
void printQueue(Queue *queue)
{
    struct Node *current = queue->front;
    printf("Queue: ");
    while (current != NULL)
    {
        printf("%d ", current->pData.id);
        current = current->next;
    }
    printf("\n");
}