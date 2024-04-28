#include <signal.h>
#include "headers.h"
#include "./datastructures/Queue.h"
#include "./communication/msg_queue.h"

Queue *queue;

void clearResources(int);
void readInputFile(char *inputPath);
bool validateArguments(int schedAlgo, int quantum);
void incRecv(int sig_num);
int safeToDestroyMsgQueue = 0;
int runningTime, arrivalTime, priority, id;
int recvProcesses = 0;
int nProcesses = 0;

int main(int argc, char * argv[])
{
    signal(SIGINT, clearResources);
    signal(SIGUSR1, incRecv);
    // TODO Initialization
    queue = constructQueue(); 
    // 1. Read the input files.
    readInputFile("processes.txt");
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    int schedAlgo, quantum = -1;
    printf("Choode Scheduling Algorithm:\n1: HPF \n2: SRTN \n3: RR\n");
    scanf("%d", &schedAlgo);
    if(schedAlgo == 3) 
    {
        printf("Enter RR quantum: ");
        scanf("%d", &quantum);
    }
    validateArguments(schedAlgo, quantum);
    
    constructMsgQueue();
    
    // 3. Initiate and create the scheduler and clock processes.
    int clk_pid = fork();
    if(clk_pid == -1) 
    {
        printf("Error in fork clk\n");
    }
    if(clk_pid == 0) 
    {
        execvp("./clk.out", (char *[]){"./clk.out", NULL});
    }
    int sched_pid = fork();
    if(sched_pid == -1) 
    {
        printf("Error in fork scheduler\n");
    }
    char algo_str[32];
        char quantum_str[32];
        sprintf(algo_str,  "%d", schedAlgo);
        sprintf(quantum_str, "%d", quantum);
    if(sched_pid == 0) 
    {
        // #TODO HERE WE WILL SEND SOME ARGS TO THE SCHEDULER
        execvp("./scheduler.out", (char *[]){"./scheduler.out",algo_str,quantum_str, NULL});
        printf("Error in exe the scheduler\n");
    }
    
    
    // 4. Use this function after creating the clock process to initialize clock
    initClk();


    // To get time use this
    int x = getClk();
    printf("current time is %d\n-------------------------------------------------------------------\n", x);
    // TODO Generation Main Loop
    while (!empty(queue))
    {
        // 5. Create a data structure for processes and provide it with its parameters.
        ProcessData prcss = pop(queue)->pData;
        while(prcss.arrivaltime != x)
        {
            x = getClk();
        }
        // 6. Send the information to the scheduler at the appropriate time.
        MsgBuff newMsg;
        newMsg.mtype = 12;
        newMsg.process = prcss;
        printf("GEN Sending: PROCESS ID: %d\n", prcss.id);
        sendMsg(newMsg);
        // printf("Process is sent successfully id : %d\n", prcss.id);
        // send a signal to scheduler telling that new process has been sent
        int tmp = recvProcesses;
        kill(sched_pid, SIGUSR2);
        while (tmp==recvProcesses);
        
    }
    // TODO : We need to send a signal to scheduler (when no processes left)
    printf("Sleeping %d ..... \n", getClk());
    while (nProcesses != recvProcesses)
    {
        sleep(1);
    }
    
    printf("Wakeup %d ..... \n", getClk());

    kill(sched_pid, SIGUSR1);

    // clear resources safely and make handler simple as possible
    while(!safeToDestroyMsgQueue);
    destroyMsgQueue();

    // 7. Clear clock resources
    int status;
    waitpid(sched_pid, &status, 0);
    if (WIFEXITED(status)) 
    {
        printf("Scheduler exited with status %d\n", WEXITSTATUS(status));
    }
    destroyClk(1);
    return 0;
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
    printf("Termination After Sending All Msgs, Please wait ....");
    safeToDestroyMsgQueue = 1;
    signal(SIGINT, clearResources);
}
void incRecv(int sig_num)
{
    recvProcesses++;
    signal(SIGUSR1, incRecv);
}
void readInputFile( char *path) 
{
    FILE* file = fopen(path, "r");
    if(file == NULL) 
    {
        printf("Error reading the input!");
        exit(-1);
    }
    char input[50];
    while (fgets(input, 50, file))
    {
        if(input[0]=='#') continue;
        nProcesses++;
        sscanf(input, "%d %d %d %d", &id, &arrivalTime, &runningTime, &priority);
        ProcessData *tmp = constructProcess(id, arrivalTime, runningTime, priority);
        push(queue, tmp);
    }
    fclose(file);
}
bool validateArguments(int schedAlgo, int quantum)
{
    if(schedAlgo > 3 || schedAlgo < 1) 
    {
        printf("Please provide a valid SchedAlgo");
        exit(-1);
    }
    if(schedAlgo == 3 && quantum < 0) 
    {
        printf("Please provide a valid quantum for RR");
        exit(-1);
    }
    
    return 1;
}