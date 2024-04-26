#include "headers.h"


void pauseProcess(int signum);


int remainingtime;
bool stop ; 
int main(int agrc, char * argv[])
{
    initClk();
    signal(SIGRTMIN+1,pauseProcess) ; 
    stop = 0 ; 

    remainingtime = atoi(argv[1]); 
    
    int ind = atoi(argv[2]);

    int prvtime = getClk() ;
    while (remainingtime)
    {       
        int time = getClk() ; 
        while (prvtime==time)
        {
            time = getClk() ; 
        }
        prvtime=time;
      //  printf("current clk is : %d",getClk());
       
        remainingtime-- ; 
         printf("Process %d has consumed sec and it's remaing time: %d\n",ind , remainingtime);
        if(remainingtime <= 0) 
        { 
            break;
        }
        // this part will be edites scheduler will be responsible for that 
        /*
        // TODO: Send  signal to scheduler stating that quantum is finised
        kill(getppid(), SIGRTMIN+1);
        // TODO: raise stop signal
        */
    }
    // TODO: Send signal to scheduler stating termination
    kill(getppid(), SIGRTMIN+2);

    destroyClk(false);    
    return 0;
}


void pauseProcess(int signum){
    // if recieved this signal process should stop
    raise(SIGSTOP);
    signal(SIGRTMIN+1,pauseProcess);
}