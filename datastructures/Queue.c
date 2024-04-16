#include <stdlib.h>
#include "Queue.h"

Queue* constructQueue() 
{
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->back = NULL;
    queue->front = NULL;
    return queue;
}
bool push(Queue* queue, ProcessData pData)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->pData = pData;
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
Node* pop(Queue* queue) 
{
    if(queue->front == NULL)
    {
        return NULL;
    }
    Node* node = queue->front;
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
Node* peek(Queue *queue)
{
    if(queue->front == NULL) 
        return NULL;
    return  queue->front;
}