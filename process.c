#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
int remaining, quantum, currQuantum, time;
int prvTime;
int main(int agrc, char * argv[])
{
    initClk();
    //TODO it needs to get the remaining time from somewhere
    remainingtime = atoi(argv[1]);
    quantum = atoi(argv[2]);
    currQuantum =  quantum;
    prvTime = getClk();

    while (remainingtime > 0)
    {
        time = getClk();
        if(time != prvTime) // time is passed by one 
        {
            remaining-=1;
            if(currQuantum > 0)
            {
                currQuantum-=1;
            }
            if(!currQuantum && remaining!=0)
            {
                // TODO: Send  signal to scheduler stating that quantum is finised
                kill(getppid(), SIGRTMIN+1);
                // TODO: raise stop signal
                raise(SIGSTOP); 
                // the scheduler will send SIGCONT to process 
                // when the process recivies a SIGCONT signal
                // it will need to continueu with an updated quantum
                currQuantum = quantum;
            }
            time = getClk();
        }
        prvTime = time;
    }
    // TODO: Send signal to scheduler stating termination
    kill(getppid(), SIGCHLD);
    destroyClk(false);
    
    return 0;
}
