#include"priQueue.h" 

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
    
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->pData = *pData;
    newNode->next = NULL; 
    // check if the queu is empty 
    if(!pqueue->front){
        pqueue->front=newNode;
        pqueue->back=newNode;    
    }else{

         // place the node in it is right place 
         // we will use an approch which is 
         // when found a node with a remaining time greater than the remainig time of the new node 
         // we will place the node before it   
         {
            struct Node * ptr = pqueue->front;
            struct Node * addAfterMe = NULL ; 
            while (ptr&&ptr->pData.remainingTime<=newNode->pData.remainingTime)
            {
                addAfterMe=ptr;
                ptr=ptr->next;
            }
            if(!addAfterMe){
                // place the new node in the front ; 
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
    
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->pData = *pData;
    newNode->next = NULL; 
    // check if the queu is empty 
    if(!pqueue->front){
        pqueue->front=newNode;
        pqueue->back=newNode;    
    }else{
         // place the node in it is right place 
         // we will use an approch which is 
         // when found a node with a priorty greater than the pri of the new node 
         // we will place the node before it   
         {
            struct Node * ptr = pqueue->front;
            struct Node * addAfterMe = NULL ; 
            while (ptr&&ptr->pData.priority<=newNode->pData.priority)
            {
                addAfterMe=ptr;
                ptr=ptr->next;
            }
            if(!addAfterMe){
                // place the new node in the front ; 
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

struct Node* pop(priQueue* pqueue) 
{
    if(pqueue->front == NULL)
    {
        return NULL;
    }
    struct Node* node = pqueue->front;
    pqueue->front = pqueue->front->next;
    if(pqueue->front == NULL) 
        pqueue->back = NULL;
    return node;
}

bool empty(priQueue * pqueue) 
{
    if(pqueue->front == NULL) 
        return 1;
    return  0;
}
struct Node* peek(priQueue *pqueue)
{
    if(pqueue->front == NULL) 
        return NULL;
    return  pqueue->front;
}
