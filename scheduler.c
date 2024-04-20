#include "headers.h"
#include "./datastructures/Queue.h"
#include "./datastructures/priQueue.h"
#include "./communication/msg_queue.h"


void noComingProcsses(int signum);
int isThereProcesses = 1;

void addProcessToReady(ProcessData *prcss);
void recvProcess(int sig_num);

void processQuantumHasFinished(int signum);
void processTerminated(int signum);


void HPF();
void SRTN();
void RR(int quantum);

Queue *queue;
priQueue * pqueue ;


struct Node *runningProcess;

int schedAlgo;
int nProcesses = 0, totRunningTime = 0;
int isThereProcessRunning = 0;

int main(int argc, char * argv[])
{
    signal(SIGUSR1, noComingProcsses);
    signal(SIGUSR2, recvProcess);
    signal(SIGRTMIN+1, processQuantumHasFinished);
    signal(SIGCHLD, processTerminated);
    initClk();
    
    //TODO implement the scheduler :)
    schedAlgo = atoi(argv[1]);
    int quantum;
    if(schedAlgo == 3) 
    {
        quantum = atoi(argv[2]);
    }

    if(schedAlgo == 1)
    {
        HPF();
    }
    else if(schedAlgo == 2)
    {
        pqueue = constructPriortyQueue(); 
        SRTN();
    }
    else 
    {
        queue = constructQueue();
        RR(quantum);
    }
    //upon termination release the clock resources.
    
    destroyClk(true);
}
void noComingProcsses(int signum)
{
    isThereProcesses = 0;
    signal(SIGUSR1, noComingProcsses);
}
void processQuantumHasFinished(int signum)
{
    isThereProcessRunning = 0;
    // TODO: perform any needed calculations
    // TODO: switch to the next process
    // handle the variable isThereProcessRunning
    // .. etc
    // if the next process is previously run 
    // then we just send SIGCONT to it
    signal(SIGRTMIN+1, processQuantumHasFinished);
}
void processTerminated(int signum)
{
    // TODO: perform any needed calculations
    // TODO: switch to next process
    // handle the variable isThereProcessRunning
    // if the next process is previously run 
    // then we just send SIGCONT to it
    signal(SIGCLD, processTerminated);
}
void addProcessToReady(ProcessData *prcss)
{
    if(schedAlgo == 1)
    {

        // we will assume that HPF non preemitive 
    
        // if the assumtion is false 
        // modify it and notify that 
        // check if thier is runnig process then compare between the pri if the new process
        // has less pri so stop runnin process and send it to ready then run new process 
        
        pushHPF(pqueue,prcss);
    
    
    }
    else if(schedAlgo == 2)
    {
        // we will assume that SRTN non preemitive 
        pushSRTN(pqueue,prcss);

        // if the assumtion is false 
        // modify it and notify that 
        // check if thier is runnig process then compare between the RT if the new process
        // has less time so stop runnin process and send it to ready then run new process 

    }
    else 
    {
        // we assume that the new process will be at the back of queue
        // even if there is a process just finish its quantum
        push(queue, prcss);
    }
}
void recvProcess(int sig_num)
{
    // this is SIGUSR2 handler is executed when new process 
    // has been sent through MsgQueue
    // TODO:  recv the processes and push it in the readyQueue
    MsgBuff msg;
    int check = msgrcv(globalMsgQueueID, &msg, sizeof(msg.process), 12, !IPC_NOWAIT);
    if(check == -1)
    {
        perror("Error receving messages");
    }
    nProcesses += 1;
    totRunningTime += msg.process.runningtime;
    addProcessToReady(&msg.process);
    
}
void HPF()
{
     while (!empty(pqueue)||isThereProcessRunning||isThereProcesses)
    {
         if(empty(pqueue)) continue;
         if(!isThereProcessRunning)
        {
            struct Node * process = pop(pqueue);
            runningProcess = process;
            isThereProcessRunning=1;
            // TODO actually create and run the process
        }
    }
}
void SRTN()
{
    while (!empty(pqueue)||isThereProcessRunning||isThereProcesses)
    {
         if(empty(pqueue)) continue;
         if(!isThereProcessRunning)
        {
            struct Node * process = pop(pqueue);
            runningProcess = process;
            isThereProcessRunning=1;
            // TODO actually create and run the process
        }
    }
    
}
void RR(int quantum)
{
    // isThereProcesses will be equal to 0 if no comming processes
    while (!empty(queue) || isThereProcessRunning || isThereProcesses)
    {
        if(empty(queue)) continue;
        if(!isThereProcessRunning)
        {
            struct Node *topProcess = pop(queue);
            runningProcess = topProcess;
            isThereProcessRunning=1;
            // TODO actually create and run the process
        }
    }
    
}