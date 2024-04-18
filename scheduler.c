#include "headers.h"
#include "./communication/msg_queue.h"


void noComingProcsses(int signum);
int isThereProcesses = 1;

void recvProcess(int sig_num);

void HPF();
void STRN();
void RR(int quantum);


int schedAlgo;

int main(int argc, char * argv[])
{
    signal(SIGUSR1, noComingProcsses);
    signal(SIGUSR2, recvProcess);
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
        STRN();
    }
    else 
    {
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
void recvProcess(int sig_num)
{
    // SIGUSR2 handler is executed when new process 
    // has been sent through MsgQueue
    // TODO:  recv the processes and push it in the readyQueue
   
    
    
}
void HPF()
{

}
void STRN()
{

}
void RR(int quantum)
{

}