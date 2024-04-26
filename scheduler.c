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

void runProcess(int isRR);


void HPF();
void SRTN();
void RR(int quantum);

Queue *queue;
priQueue * pqueue ;


struct Node *runningProcess;

int schedAlgo;
int nProcesses = 0, totRunningTime = 0;
int isThereProcessRunning = 0;
int MsyQueueID;

int main(int argc, char * argv[])
{   
    signal(SIGUSR1, noComingProcsses);
    signal(SIGUSR2, recvProcess);
    signal(SIGRTMIN+1, processQuantumHasFinished);
    signal(SIGRTMIN+2, processTerminated);
    initClk();
    
    //TODO implement the scheduler :)
    schedAlgo = atoi(argv[1]);
    int quantum;
    if(schedAlgo == 3) 
    {
        quantum = atoi(argv[2]);
    }

    key_t msg_key = ftok("/home/abdallahsalah/Desktop/ScheduVerse/communication/keyfile", 'A');
    MsyQueueID = msgget(msg_key, 0666 | IPC_CREAT);

    if(schedAlgo == 1)
    {   
        pqueue=constructPriortyQueue(); 
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
    printf("Process with id %d has finished a quantum\n", runningProcess->pData.id);
    
    runningProcess->pData.state=1 ; 

    if(schedAlgo==3)
    push(queue, &(runningProcess->pData));
    if(schedAlgo==1)
    pushHPF(pqueue,&(runningProcess->pData));
    if(schedAlgo==2)
    pushSRTN(pqueue,&(runningProcess->pData));

    isThereProcessRunning = 0;
    signal(SIGRTMIN+1, processQuantumHasFinished);
}
void processTerminated(int signum)
{
    runningProcess->pData.state = 4; // TERMINATED
    int finishTime = getClk();
    int waitingTime = finishTime - runningProcess->pData.runningtime - runningProcess->pData.arrivaltime;
    double turnaroundTime = finishTime - runningProcess->pData.arrivaltime;
    double weightedTurnaroundTime = turnaroundTime/runningProcess->pData.runningtime; // check if right
    // TODO : print to the output file
    printf("------TERMINATED------>> process id: %d, arrivalTime: %d, runningTime: %d, startTiem: %d, finishTime: %d, waitingTime: %d\n", runningProcess->pData.id, runningProcess->pData.arrivaltime, 
    runningProcess->pData.runningtime, runningProcess->pData.startTime, finishTime, waitingTime);
    isThereProcessRunning = 0;
    signal(SIGRTMIN+2, processTerminated);
}
void addProcessToReady(ProcessData *prcss)
{
    if(schedAlgo == 1)
    {

        prcss->state = 1;  // READY
        pushHPF(pqueue,prcss);    
    }
    else if(schedAlgo == 2)
    {
        prcss->state = 1;  // READY
        pushSRTN(pqueue,prcss);

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
    int check = msgrcv(MsyQueueID, &msg, sizeof(msg.process), 0, IPC_NOWAIT);
    // printf("SCHED Recieve PROCESS ID: %d\n", msg.process.id);
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
            runningProcess->pData.state=2 ;
            isThereProcessRunning=1;
             if((runningProcess->pData.getCPUBefore) == 1) {
                printf("Sending SIGCONT to process %d\n",(runningProcess->pData.id));
                // send SIGCONT 
                kill(runningProcess->pData.realID, SIGCONT);
            }
            else {
                printf("Run first time the process id: %d\n", (runningProcess->pData.id));
                runProcess(runningProcess->pData.runningtime);
                // run a new process 
            }

            // TODO actually create and run the process
        }
    }
}
void SRTN()
{   
    int prvTime = getClk();
    
    // edit the algorithm to be preemitive 
    while (!priempty(pqueue)||isThereProcessRunning||isThereProcesses)
    {
        if(priempty(pqueue)) continue;
        if(!isThereProcessRunning)
        {
            prvTime=getClk(); 
            struct priNode * process = pripop(pqueue);
            runningProcess = process;
            runningProcess->pData.state=2 ;
            isThereProcessRunning=1;
            if((runningProcess->pData.getCPUBefore) == 1) {
                printf("Sending SIGCONT to process %d\n",(runningProcess->pData.id));
                // send SIGCONT 
                kill(runningProcess->pData.realID, SIGCONT);
            }
            else {
                printf("Run first time the process id: %d\n", (runningProcess->pData.id));
                runPcss();
                // run a new process 
            }
        }else{
            int time = getClk(); 
            if(time == prvTime) continue; 
            runningProcess->pData.remainingTime--;
            
            struct priNode * topprocess = pripeek(pqueue);
            // check if the top of the queue has smaller RT 
            printf("RT of running process and top is at clk : %d .. %d \n",runningProcess->pData.remainingTime,topprocess->pData.remainingTime);
            if(topprocess&&(runningProcess->pData.remainingTime > topprocess->pData.remainingTime ) ){
                // stop it and return it to  the ready queue 
                kill(runningProcess->pData.realID,SIGRTMIN+1);
                addProcessToReady(&runningProcess->pData);
                isThereProcessRunning=0 ; 
            }
            prvTime=time ; 
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
            runningProcess->pData.state=2 ; 
            isThereProcessRunning=1;
            if((runningProcess->pData.getCPUBefore) == 1) {
                printf("Sending SIGCONT to process %d\n",(runningProcess->pData.id));
                // send SIGCONT 
                kill(runningProcess->pData.realID, SIGCONT);
            }
            else {
                printf("Run first time the process id: %d\n", (runningProcess->pData.id));
                runProcess(quantum);
                // run a new process 
            }
            // TODO actually create and run the process
        }
    }
    
}
void runProcess(int isRR) {
    if(!isRR)  // not RR || last process in RR 
    {
        isRR = runningProcess->pData.runningtime;
    }
    char runningtime_str[32];
    char isRR_str[32];
    sprintf(runningtime_str, "%d", (runningProcess->pData.runningtime));
    sprintf(isRR_str, "%d", isRR);
    int pid = fork();
    
    if(pid == 0) 
    {
        execvp("./process.out", (char *[]){"./process.out", runningtime_str, isRR_str, NULL});
    }

    runningProcess->pData.realID = pid;
    runningProcess->pData.startTime = getClk();
    runningProcess->pData.state = 2; // Running
    runningProcess->pData.getCPUBefore = 1;
   
}

void runPcss(){
    char runningtime_str[32];
    char ind[32] ; 
    sprintf(runningtime_str, "%d", (runningProcess->pData.runningtime));
    sprintf(ind,"%d",(runningProcess->pData.id));
    int pid = fork();
    
    if(pid == 0) 
    {
        execvp("./pcss.out", (char *[]){"./pcss.out", runningtime_str,ind, NULL});
    }

    runningProcess->pData.realID = pid;
    runningProcess->pData.startTime = getClk();
    runningProcess->pData.state = 2; // Running 
    runningProcess->pData.getCPUBefore = 1;
}