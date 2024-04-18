#include "headers.h"
#include "./datastructures/Queue.h"
#include "./communication/msg_queue.h"

Queue *queue;

void clearResources(int);
void readInputFile(char *inputPath);
bool validateArguments(int *schedAlgo, int *quantum);

int safeToDestroyMsgQueue = 0;

int main(int argc, char * argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    queue = constructQueue(); 
    // 1. Read the input files.
    readInputFile("processes.txt");
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    int schedAlgo, quantum;
    printf("Choode Scheduling Algorithm:\n 1: HPF \n 2: SRTN \n 3: RR \n");
    scanf("%d", &schedAlgo);
    if(schedAlgo == 3) 
    {
        printf("Enter RR quantum: ");
        scanf("%d", &quantum);
    }
    // 3. Initiate and create the scheduler and clock processes.
    int clk_pid = fork();
    if(clk_pid == 0) 
    {
        printf("Initiate the clk"); 
        execv("./clk.out", (char *[]){"./clk.out", NULL});
    }
    else 
    {
        printf("Error in fork() to execv the clk");
        exit(-1);
    }
    int sched_pid = fork();
    if(sched_pid == 0) 
    {
        printf("Initiate the Scheduler"); 
        // #TODO HERE WE WILL SEND SOME ARGS TO THE SCHEDULER
        execv("./scheduler.out", (char *[]){"./scheduler.out", NULL});
    }
    else 
    {
        printf("Error in fork() to execv the scheduler");
        exit(-1);
    }
    // 4. Use this function after creating the clock process to initialize clock
    initClk();
    // To get time use this
    int x = getClk();
    printf("current time is %d\n", x);
     
    constructMsgQueue();
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
        MsgBuff msg = constructMsg(prcss);
        sendMsg(msg);
    }
    // TODO : We need to send a signal to scheduler (no processes left)

    // 7. Clear clock resources
    destroyClk(true);
    // clear resources safely and make handler simple as possible
    while(!safeToDestroyMsgQueue);
    while(!isMsgQueueEmpty())
    {
        // AVOID PULLING IN LOOPS
        sleep(2);
    }
    destroyMsgQueue();
    return 0;
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
    printf("Termination After Sending All Msgs, Please wait ....");
    safeToDestroyMsgQueue = 1;
    signal(SIGINT, clearResources);
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

        ProcessData tmp;
        sscanf(input, "%d %d %d %d", &tmp.id, &tmp.arrivaltime, &tmp.runningtime, &tmp.priority);
        push(queue, tmp);
    }
    fclose(file);
}
bool validateArguments(int *schedAlgo, int *quantum)
{
    if(*schedAlgo > 3 || *schedAlgo < 1) 
    {
        printf("Please provide a valid SchedAlgo");
        return 0;
    }
    if(*schedAlgo == 3 && *quantum < 0) 
    {
         printf("Please provide a valid quantum for RR");
        return 0;
    }
    
    return 1;
}