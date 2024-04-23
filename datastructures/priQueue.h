#pragma once
#include <stdbool.h>
#include <stdlib.h>

#include "../processData.h"


struct priNode
{
    ProcessData pData;
    struct priNode *next;
};

struct 
{
    struct priNode *front;
    struct priNode *back;
}typedef priQueue ;

priQueue * constructPriortyQueue(){
    priQueue * pQueue = (priQueue*)malloc(sizeof(priQueue));
    pQueue->front = NULL; 
    pQueue->back = NULL;
    return pQueue ;  
}


//to use the same pri queue files for both this fuction must have two version for ex HPFpush and SRTpush 
// TODO: implement HPFpush 
// the following implementation is for SRTpush version
bool pushSRTN(priQueue* pqueue, ProcessData *pData){
    
    struct priNode* newNode = (struct priNode*)malloc(sizeof(struct priNode));
    newNode->pData = *pData;
    newNode->next = NULL; 
    // check if the queu is empty 
    if(!pqueue->front){
        pqueue->front=newNode;
        pqueue->back=newNode;    
    }else{

         // place the priNode in it is right place 
         // we will use an approch which is 
         // when found a priNode with a remaining time greater than the remainig time of the new priNode 
         // we will place the priNode before it   
         {
            struct priNode * ptr = pqueue->front;
            struct priNode * addAfterMe = NULL ; 
            while (ptr&&ptr->pData.remainingTime<=newNode->pData.remainingTime)
            {
                addAfterMe=ptr;
                ptr=ptr->next;
            }
            if(!addAfterMe){
                // place the new priNode in the front ; 
                newNode->next=pqueue->front; 
                pqueue->front=newNode; 
            }else{
                //place the newnode 
                newNode->next=ptr;
                addAfterMe->next=newNode;
            }
            
         }
   
    }

    return 1 ;       
}

bool pushHPF(priQueue* pqueue, ProcessData *pData){
    
    struct priNode* newNode = (struct priNode*)malloc(sizeof(struct priNode));
    newNode->pData = *pData;
    newNode->next = NULL; 
    // check if the queu is empty 
    if(!pqueue->front){
        pqueue->front=newNode;
        pqueue->back=newNode;    
    }else{
         // place the priNode in it is right place 
         // we will use an approch which is 
         // when found a priNode with a priorty greater than the pri of the new priNode 
         // we will place the priNode before it   
         {
            struct priNode * ptr = pqueue->front;
            struct priNode * addAfterMe = NULL ; 
            while (ptr&&ptr->pData.priority<=newNode->pData.priority)
            {
                addAfterMe=ptr;
                ptr=ptr->next;
            }
            if(!addAfterMe){
                // place the new priNode in the front ; 
                newNode->next=pqueue->front; 
                pqueue->front=newNode; 
            }else{
                //place the newnode 
                newNode->next=ptr;
                addAfterMe->next=newNode;
            }
            
         }
   
    }

    return 1 ;       
}

struct priNode* pripop(priQueue* pqueue) 
{
    if(pqueue->front == NULL)
    {
        return NULL;
    }
    struct priNode* priNode = pqueue->front;
    pqueue->front = pqueue->front->next;
    if(pqueue->front == NULL) 
        pqueue->back = NULL;
    return priNode;
}

bool priempty(priQueue * pqueue) 
{
    if(pqueue->front == NULL) 
        return 1;
    return  0;
}
struct priNode* pripeek(priQueue *pqueue)
{
    if(pqueue->front == NULL) 
        return NULL;
    return  pqueue->front;
}
