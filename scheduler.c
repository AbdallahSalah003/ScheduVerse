#include <math.h>
#include "headers.h"
#include "./datastructures/Queue.h"
#include "./datastructures/priQueue.h"
#include "./communication/msg_queue.h"
#include "./datastructures/dynamicArray.h"


void noComingProcsses(int signum);
int isThereProcesses = 1;

void addProcessToReady(ProcessData *prcss);
void recvProcess(int sig_num);

void processQuantumHasFinished();
void processTerminated(int signum);

void runProcess();

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
int RR_isOn = 0;
int quantum;
FILE *file_ptr;
double total_W = 0, total_WTA = 0, total_run = 0;
 dynamic_array* storeWTA;

int main(int argc, char * argv[])
{   
    printf("The Scheduler is executed!\n");
    signal(SIGUSR1, noComingProcsses);
    signal(SIGUSR2, recvProcess);
    signal(SIGRTMIN+2, processTerminated);
    initClk();
    
    //TODO implement the scheduler :)
    schedAlgo = atoi(argv[1]);
    if(schedAlgo == 3) 
    {
        quantum = atoi(argv[2]);
    }

    key_t msg_key = ftok("/home/abdallahsalah/Desktop/ScheduVerse/communication/keyfile", 'A');
    MsyQueueID = msgget(msg_key, 0666 | IPC_CREAT);


    
    char filename[] = "/home/abdallahsalah/Desktop/ScheduVerse/scheduler.log";
    file_ptr = fopen(filename, "a");
    if (file_ptr == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    fprintf(file_ptr, "#At time x process y stopped arr w total z remain y wait k\n");


    arrayInit(&storeWTA); 


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
        RR_isOn = 1;
        queue = constructQueue();
        RR(quantum);
    }
    printf("HELLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL\n");
    double avgWTA = total_WTA/nProcesses;
    double sum = 0;
    for (int i = 0; i < storeWTA->size; i++) { 
        printf("%lf \n", storeWTA->array[i]);
        sum += (storeWTA->array[i] - avgWTA) * (storeWTA->array[i] - avgWTA); 
    } 
    char prefPath[] = "/home/abdallahsalah/Desktop/ScheduVerse/scheduler.pref";
    FILE *pref_ptr = fopen(prefPath, "a");
    fprintf(pref_ptr, "CPU Utilization = %.2lf %% \n", (total_run/getClk())*100.0);
    fprintf(pref_ptr, "Avg WTA = %.2lf \n", avgWTA);
    fprintf(pref_ptr, "Avg Waiting = %.2lf \n", total_W/nProcesses);
    fprintf(pref_ptr, "Std WTA = %.2lf \n", sqrt(sum/nProcesses));

    //upon termination release the clock resources.
    kill(getppid(), SIGINT);
    fclose(file_ptr);
    fclose(pref_ptr);
    freeArray(storeWTA);
    destroyClk(false);
}
void noComingProcsses(int signum)
{
    isThereProcesses = 0;
    signal(SIGUSR1, noComingProcsses);
}
void processQuantumHasFinished()
{
    printf("Process with id %d has finished a quantum with remaining time %d\n", runningProcess->pData.id, runningProcess->pData.remainingTime);
    printf("CURRENT TIME : %d\n", getClk());
    fprintf(file_ptr, "At time %d process %d stopped arr %d total %d remain %d wait %d\n", getClk(), runningProcess->pData.id,
    runningProcess->pData.arrivaltime, runningProcess->pData.runningtime, runningProcess->pData.remainingTime, -1);
    
    runningProcess->pData.state=1 ; // READY
    push(queue, &(runningProcess->pData));
    isThereProcessRunning = 0;
}
void processTerminated(int signum)
{
    runningProcess->pData.state = TERMINATED;
    int finishTime = getClk();
    int waitingTime = finishTime - runningProcess->pData.runningtime - runningProcess->pData.arrivaltime;
    double turnaroundTime = finishTime - runningProcess->pData.arrivaltime;
    double weightedTurnaroundTime = turnaroundTime/runningProcess->pData.runningtime; // check if right
    total_W += waitingTime;
    total_WTA +=weightedTurnaroundTime;
    
    printf("------TERMINATED---> processs id %d and finish time %d \n ", runningProcess->pData.id, finishTime);
    // TODO : print to the output file
    fprintf(file_ptr, "At time %d process %d finished arr %d total %d remain %d wait %d TA %.0lf WTA %.2lf\n", finishTime, runningProcess->pData.id, 
    runningProcess->pData.arrivaltime, runningProcess->pData.runningtime, runningProcess->pData.remainingTime, 
    waitingTime, turnaroundTime, weightedTurnaroundTime);
    if(RR_isOn) printQueue(queue);
    isThereProcessRunning = 0;

    insertItem(storeWTA, weightedTurnaroundTime);

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
                runProcess();
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
                fprintf(file_ptr, "At time %d process %d resumed arr %d total %d remain %d wait %d\n", getClk(), runningProcess->pData.id,
                runningProcess->pData.arrivaltime, runningProcess->pData.runningtime, runningProcess->pData.remainingTime, -1);
            }
            else {
                printf("Run first time the process id: %d\n", (runningProcess->pData.id));
                runProcess();
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
                kill(runningProcess->pData.realID, SIGSTOP);
                fprintf(file_ptr, "At time %d process %d stopped arr %d total %d remain %d wait %d\n", getClk(), runningProcess->pData.id,
                runningProcess->pData.arrivaltime, runningProcess->pData.runningtime, runningProcess->pData.remainingTime, -1);
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
                fprintf(file_ptr, "At time %d process %d resumed arr %d total %d remain %d wait %d\n", getClk(), runningProcess->pData.id,
                runningProcess->pData.arrivaltime, runningProcess->pData.runningtime, runningProcess->pData.remainingTime, -1);
            }
            else {
                printf("Run first time the process id: %d\n", (runningProcess->pData.id));
                runProcess();
            }
            // After quantum finish this function send a SIGSTOP to running process
            trackRunningProcess();
        }
    }
    
}
void runProcess() {
    char runningtime_str[32];
    char ind[32] ; 
    sprintf(runningtime_str, "%d", (runningProcess->pData.runningtime));
    sprintf(ind,"%d",(runningProcess->pData.id));
    int pid = fork();
    
    if(pid == 0) 
    {
        execvp("./process.out", (char *[]){"./process.out", runningtime_str,ind, NULL});
    }
     fprintf(file_ptr, "At time %d process %d started arr %d total %d remain %d wait %d\n", getClk(), runningProcess->pData.id,
    runningProcess->pData.arrivaltime, runningProcess->pData.runningtime, runningProcess->pData.remainingTime, -1);
    runningProcess->pData.realID = pid;
    runningProcess->pData.startTime = getClk();
    runningProcess->pData.state = 2; // Running 
    runningProcess->pData.getCPUBefore = 1;
    total_run += runningProcess->pData.runningtime;
}
void trackRunningProcess() {
    int start_time = getClk();
    int curr_time, time;
    if(runningProcess->pData.remainingTime <= quantum) 
    {
        time = runningProcess->pData.remainingTime;   
        while(1) {
            curr_time = getClk();
            while (start_time==curr_time)
            {
                curr_time = getClk(); 
            }
            start_time=curr_time;       
            time--; 
            if(time <= 0) 
            { 
                break;
            }
        }
        // sleep(runningProcess->pData.remainingTime);
        runningProcess->pData.remainingTime = 0;
    }
    else 
    {
        time = quantum;
        while(1) {
            curr_time = getClk();
            while (start_time==curr_time)
            {
                curr_time = getClk(); 
            }
            start_time=curr_time;       
            time--; 
            if(time <= 0) 
            { 
                break;
            }
        }
        // sleep(quantum);
        kill(runningProcess->pData.realID, SIGSTOP);  
        // Update process remaining time   
        runningProcess->pData.remainingTime -= quantum;
        processQuantumHasFinished();
    }
}
