#include "headers.h"
#include "./datastructures/Queue.h"
#include "./communication/msg_queue.h"


void noComingProcsses(int signum);
int isThereProcesses = 1;

void addProcessToReady(ProcessData *prcss);
void recvProcess(int sig_num);

void HPF();
void STRN();
void RR(int quantum);

Queue *queue;

struct Node *runningProcess;

int schedAlgo;
int nProcesses = 0, totRunningTime = 0;

int main(int argc, char * argv[])
{
    signal(SIGUSR1, noComingProcsses);
    signal(SIGUSR2, recvProcess);
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
        STRN();
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
void addProcessToReady(ProcessData *prcss)
{
    if(schedAlgo == 1)
    {

    }
    else if(schedAlgo == 2)
    {

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

}
void STRN()
{

}
void RR(int quantum)
{
    int running = 0;
    int WAIT = 1; // if queue is empty and there is no running process
    // we must not stop the algorithm until there is no comming processes
    // thus we use the var WAIT = 1;
    while (!empty(queue) || running || WAIT)
    {
        if(empty(queue)) continue;
        if(!running)
        {
            struct Node *topProcess = pop(queue);
            runningProcess = topProcess;
            // TODO actually create and run the process
        }
    }
    
}