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

void runProcess(ProcessData *pData,int isRR);


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
    printf("iam here");
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
    push(queue, &(runningProcess->pData));
    isThereProcessRunning = 0;
    signal(SIGRTMIN+1, processQuantumHasFinished);
}
void processTerminated(int signum)
{
    runningProcess->pData.state = 4; // TERMINATED
    int finishTime = getClk();
    int waitingTime = finishTime - runningProcess->pData.runningtime;
    double turnaroundTime = finishTime - runningProcess->pData.arrivaltime;
    double weightedTurnaroundTime = turnaroundTime/runningProcess->pData.runningtime; // check if right
    // TODO : print to the output file

    isThereProcessRunning = 0;
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
        prcss->state = 1;  // READY
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
     while (!priempty(pqueue)||isThereProcessRunning||isThereProcesses)
    {
         if(priempty(pqueue)) continue;
         if(!isThereProcessRunning)
        {
            struct priNode * process = pripop(pqueue);
            runningProcess = process;
            isThereProcessRunning=1;
            // TODO actually create and run the process
        }
    }
}
void SRTN()
{
    while (!priempty(pqueue)||isThereProcessRunning||isThereProcesses)
    {
         if(priempty(pqueue)) continue;
         if(!isThereProcessRunning)
        {
            struct priNode * process = pripop(pqueue);
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
            if(runningProcess->pData.getCPUBefore) {
                // send SIGCONT 
                kill(SIGCONT, runningProcess->pData.realID);
            }
            else {
                
                runProcess(&(runningProcess->pData), quantum);
                // run a new process 
            }
            // TODO actually create and run the process
        }
    }
    
}
void runProcess(ProcessData *pData, int isRR) {
    if(!isRR) 
    {
        isRR = pData->runningtime;
    }

    int pid = fork();
    
    if(pid == 0) 
    {
        execv("./process.out", (char *[]){"./process.out", (char *)pData->runningtime, (char *)isRR});
    }
            runningProcess->pData.realID = pid;
            runningProcess->pData.startTime = getClk();
            runningProcess->pData.state = 2; // READY
            runningProcess->pData.getCPUBefore = 1;

    // pid realID of the running process
    /*
    int num = 42;
    char str[ENOUGH]; // Define a buffer to hold the string
    sprintf(str, "%d", num);
    */
   // TODO : set pData->realID int the PCB
   // make any calculations and change values in PCB
   
}