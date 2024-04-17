#include "headers.h"
#include "./datastructures/Queue.h"

Queue *queue;

void clearResources(int);
void readInputFile(char *inputPath);
bool validateArguments(int *schedAlgo, int *quantum);

int main(int argc, char * argv[])
{
    /*
    int *schedAlgo, *quantum;
    bool chechArgs = validateArguments(argc, argv, schedAlgo, quantum);
    if(!chechArgs) 
    {
        exit(-1);
    }
    */
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
    // 4. Use this function after creating the clock process to initialize clock
        // initClk();
    // To get time use this
        // int x = getClk();
        // printf("current time is %d\n", x);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    // 7. Clear clock resources
        // destroyClk(true);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
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