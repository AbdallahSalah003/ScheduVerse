#include "./headers.h"
#include "./datastructures/priQueue.h"
#include "./communication/msg_queue.h"
#include "./datastructures/dynamicArray.h"
#include "./communication/semaphores.h"
#include "./datastructures/Queue.h"

void noComingProcsses(int signum);
int isThereProcesses = 1;

void addProcessToReady(ProcessData *prcss);
void recvProcess(int sig_num);

void processTerminated(int signum);

void runProcess();

void HPF();
void SRTN();
void RR(int quantum);

double sqrt(double x) {
    double epsilon = 0.001; 
    double guess = 1.0; 
    double prev_guess;
    do {
        prev_guess = guess;
        guess = (guess + x / guess) / 2;
    } while (prev_guess - guess > epsilon || guess - prev_guess > epsilon);
    return guess;
}

priQueue * pqueue ;
Queue *queue;

struct Node *runningProcess;
ProcessData *runningP__RR;

int schedAlgo;
int nProcesses = 0, totRunningTime = 0;
int isThereProcessRunning = 0;
int MsyQueueID;
int RR_isOn = 0;
int quantum;
FILE *file_ptr;
double total_W = 0, total_WTA = 0, total_run = 0;
 dynamic_array* storeWTA;
int flag_termination = 0;
int semaphore_id;

int main(int argc, char * argv[])
{   
    signal(SIGUSR1, noComingProcsses);
    signal(SIGUSR2, recvProcess);
    signal(SIGRTMIN+2, processTerminated);
    initClk();
    
    printf("The Scheduler is executed!\n");
    //TODO implement the scheduler :)
    schedAlgo = atoi(argv[1]);
    if(schedAlgo == 3) 
    {
        quantum = atoi(argv[2]);
    }

    key_t msg_key = ftok("/home/abdallahsalah/Desktop/ScheduVerse/communication/keyfile", 'A');
    MsyQueueID = msgget(msg_key, 0666 | IPC_CREAT);

    semaphore_id = constructSemaphore();
    
    char filename[] = "/home/abdallahsalah/Desktop/ScheduVerse/scheduler.log";
    file_ptr = fopen(filename, "w");
    if (file_ptr == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    fprintf(file_ptr, "#At time x process y stopped arr w total z remain y wait k\n");


    arrayInit(&storeWTA); 

    printf("Choosing the scheduling algo now ...\n");
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

    double avgWTA = total_WTA/nProcesses;
    double sum = 0;
    for (int i = 0; i < storeWTA->size; i++) { 
        sum += (storeWTA->array[i] - avgWTA) * (storeWTA->array[i] - avgWTA); 
    } 
    char prefPath[] = "/home/abdallahsalah/Desktop/ScheduVerse/scheduler.pref";
    FILE *pref_ptr = fopen(prefPath, "w");
    fprintf(pref_ptr, "CPU Utilization = %.2lf %% \n", (total_run/getClk())*100.0);
    fprintf(pref_ptr, "Avg WTA = %.2lf \n", avgWTA);
    fprintf(pref_ptr, "Avg Waiting = %.2lf \n", total_W/nProcesses);
    fprintf(pref_ptr, "Std WTA = %.2lf \n", sqrt(sum/nProcesses));

    //upon termination release the clock resources.
    fclose(file_ptr);
    fclose(pref_ptr);
    freeArray(storeWTA);
    destroyClk(0);
    deleteSemaphore(semaphore_id);
    kill(getppid(), SIGINT);
}
void noComingProcsses(int signum)
{
    isThereProcesses = 0;
    signal(SIGUSR1, noComingProcsses);
}
void processTerminated(int signum)
{
    int finishTime = getClk(), waitingTime;
    double turnaroundTime, weightedTurnaroundTime;
    runningProcess->pData.state = TERMINATED;
    runningProcess->pData.remainingTime = 0;
    waitingTime = finishTime - runningProcess->pData.runningtime - runningProcess->pData.arrivaltime;
    turnaroundTime = finishTime - runningProcess->pData.arrivaltime;
    weightedTurnaroundTime = turnaroundTime/runningProcess->pData.runningtime; // check if right
    printf("------TERMINATED---> processs id %d and finish time %d \n ", runningProcess->pData.id, finishTime);
    // TODO : print to the output file
    fprintf(file_ptr, "At time %d process %d finished arr %d total %d remain %d wait %d TA %.0lf WTA %.2lf\n", finishTime, runningProcess->pData.id, 
    runningProcess->pData.arrivaltime, runningProcess->pData.runningtime, runningProcess->pData.remainingTime, 
    waitingTime, turnaroundTime, weightedTurnaroundTime);
    
    total_W += waitingTime;
    total_WTA +=weightedTurnaroundTime;
    
    
    flag_termination = 1;

    insertItem(storeWTA, weightedTurnaroundTime);

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
    printf("SCHED Recieve PROCESS ID: %d\n", msg.process.id);
    if(check == -1)
    {
        perror("Error receving messages");
    }
    nProcesses += 1;
    totRunningTime += msg.process.runningtime;   
    down(semaphore_id);
    printf("Im Process with id %d having the lock now\n", msg.process.id);
    addProcessToReady(&msg.process);
    up(semaphore_id);
        // signal to inform generator that schedu recieved a message
    kill(getppid(), SIGUSR1);
    printf("Sched sent a signal to generator inform with reciving\n");

    signal(SIGUSR2, recvProcess);    
}
void HPF()
{
     while (!priempty(pqueue)||isThereProcessRunning||isThereProcesses)
    {
         if(priempty(pqueue)) continue;
         if(!isThereProcessRunning)
        {
            down(semaphore_id);
            struct priNode * process = pripop(pqueue);
            up(semaphore_id);
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
            down(semaphore_id);
            struct priNode * process = pripop(pqueue);
            up(semaphore_id);
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
            down(semaphore_id);
            struct priNode * topprocess = pripeek(pqueue);
            up(semaphore_id);
            // check if the top of the queue has smaller RT 
            printf("RT of running process and top is at clk : %d .. %d \n",runningProcess->pData.remainingTime,topprocess->pData.remainingTime);
            if(topprocess&&(runningProcess->pData.remainingTime > topprocess->pData.remainingTime ) ){
                // stop it and return it to  the ready queue 
                kill(runningProcess->pData.realID, SIGSTOP);
                fprintf(file_ptr, "At time %d process %d stopped arr %d total %d remain %d wait %d\n", getClk(), runningProcess->pData.id,
                runningProcess->pData.arrivaltime, runningProcess->pData.runningtime, runningProcess->pData.remainingTime, -1);
                down(semaphore_id);
                addProcessToReady(&runningProcess->pData);
                up(semaphore_id);
                isThereProcessRunning=0 ; 
            }
            prvTime=time ; 
        }
    }
    
}
void RR(int quantum)
{
    printf("Starting RR Algorithm Now ....\n");
    // isThereProcesses will be equal to 0 if no comming processes
    runningProcess =  malloc(sizeof(struct Node));
    while (!empty(queue) || isThereProcessRunning || isThereProcesses)
    {
        if(empty(queue)) continue;
        if(!isThereProcessRunning)
        {
            down(semaphore_id);
            runningProcess = pop(queue);
            up(semaphore_id);
            runningProcess->pData.state = RUNNING; 
            isThereProcessRunning=1;
            // printf("GET CPU before for process %d\n",runningProcess->pData.getCPUBefore);
            if((runningProcess->pData.getCPUBefore) == 1) {
                printf("Sending SIGCONT to process %d\n",(runningProcess->pData.id));
                // send SIGCONT 
                kill(runningProcess->pData.realID, SIGCONT);
                int waitTime = getClk() - (runningProcess->pData.runningtime - runningProcess->pData.remainingTime) - runningProcess->pData.arrivaltime;
                fprintf(file_ptr, "At time %d process %d resumed arr %d total %d remain %d wait %d\n", getClk(), runningProcess->pData.id,
                runningProcess->pData.arrivaltime, runningProcess->pData.runningtime, runningProcess->pData.remainingTime, waitTime);
            }
            else {
                printf("Run first time the process id: %d\n", (runningProcess->pData.id));
                runProcess();
            }
            // After quantum finish this function send a SIGSTOP to running process
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
                kill(runningProcess->pData.realID, SIGSTOP);  
                // Update process remaining time   
                runningProcess->pData.remainingTime -= quantum;
                int waitTime = getClk() - (runningProcess->pData.runningtime - runningProcess->pData.remainingTime) - runningProcess->pData.arrivaltime;
                printf("Time : %d, Process with id %d has finished a quantum with remaining time %d\n",getClk(), runningProcess->pData.id, runningProcess->pData.remainingTime);
                fprintf(file_ptr, "At time %d process %d stopped arr %d total %d remain %d wait %d\n", getClk(), runningProcess->pData.id,
                runningProcess->pData.arrivaltime, runningProcess->pData.runningtime, runningProcess->pData.remainingTime, waitTime);
    
                runningProcess->pData.state=1 ; // READY
                isThereProcessRunning = 0;
                // move current of the circuler queue
                down(semaphore_id);
                printf("Im Process with id %d having the lock now\n", runningProcess->pData.id);
                push(queue, &(runningProcess->pData));
                up(semaphore_id);
                printf("I'm Process with id %d release the lock \n", runningProcess->pData.id);
            }
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
    int waitTime = getClk() - runningProcess->pData.arrivaltime;
    fprintf(file_ptr, "At time %d process %d started arr %d total %d remain %d wait %d\n", getClk(), runningProcess->pData.id,
    runningProcess->pData.arrivaltime, runningProcess->pData.runningtime, runningProcess->pData.remainingTime, waitTime);
    runningProcess->pData.realID = pid;
    runningProcess->pData.startTime = getClk();
    runningProcess->pData.state = 2; // Running 
    runningProcess->pData.getCPUBefore = 1;
    total_run += runningProcess->pData.runningtime;
}