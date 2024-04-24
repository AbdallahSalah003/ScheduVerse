#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
int remaining, quantum, currQuantum, time;
int prvTime;
int min(int a, int b) {
    if (a < b)
        return a;
    else
        return b;
}

int main(int agrc, char * argv[])
{
    initClk();
    //TODO it needs to get the remaining time from somewhere
    remainingtime = atoi(argv[1]);
    quantum = atoi(argv[2]);
    currQuantum =  quantum;
    prvTime = getClk();
    while (1)
    {
        time = getClk();
        int compare = min(remainingtime, quantum);
        while (prvTime+compare>time)
        {
            time = getClk();
        }
        remainingtime -= compare;
        if(remainingtime <= 0) {
            break;
        }
        else {
            // TODO: Send  signal to scheduler stating that quantum is finised
            kill(getppid(), SIGRTMIN+1);
            // TODO: raise stop signal
            raise(SIGSTOP); 
            // the scheduler will send SIGCONT to process 
        }
    }
    // TODO: Send signal to scheduler stating termination
    kill(getppid(), SIGRTMIN+2);

    destroyClk(false);    
    return 0;
}
