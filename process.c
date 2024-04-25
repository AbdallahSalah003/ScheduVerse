#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
int quantum;
int min(int a, int b) {
    if (a < b)
        return a;
    else
        return b;
}
void processTime(int time);
int main(int agrc, char * argv[])
{
    initClk();
    //TODO it needs to get the remaining time from somewhere
    remainingtime = atoi(argv[1]);
    quantum = atoi(argv[2]);
    printf("Process with remaining time : %d has started a quantum\n", remainingtime);
    while (1)
    {
        int compare = min(remainingtime, quantum);
        processTime(compare);
        remainingtime -= compare;
        if(remainingtime <= 0) 
        { 
            break;
        }
        // TODO: Send  signal to scheduler stating that quantum is finised
        kill(getppid(), SIGRTMIN+1);
        // TODO: raise stop signal
        printf("Process has finished a quantum with remaing time : %d\n", remainingtime);
        raise(SIGSTOP); 
        // the scheduler will send SIGCONT to process 
    }
    // TODO: Send signal to scheduler stating termination
    kill(getppid(), SIGRTMIN+2);

    destroyClk(false);    
    return 0;
}
void processTime(int time) {
    int startTime = getClk();
    int currTime;
    while(time > 0) {
        currTime = getClk();
        if(startTime == currTime) continue;
        startTime = getClk();
        time--;
    }
}