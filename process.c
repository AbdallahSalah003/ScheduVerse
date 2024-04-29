#include "headers.h"





int remainingtime;

int main(int agrc, char * argv[])
{
    initClk();
    

    remainingtime = atoi(argv[1]); 
    
    int ind = atoi(argv[2]);
    int prvtime = getClk();
    while (remainingtime)
    {       
        int time = getClk(); 
        if(prvtime!=time)
            prvtime=time;
        while (prvtime==time)
        {
            time = getClk() ; 
        }
        prvtime=time;
      //  printf("current clk is : %d",getClk());
        remainingtime-- ; 
        if(remainingtime<=3)
            printf("\nprocess 2 realistically has %d left now",remainingtime);
        //  printf("Process %d has consumed sec and it's remaing time: %d\n",ind , remainingtime);
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
    printf("Process with ID %d Terminating and sending signal the scheduler telling termination \n", ind);
    // TODO: Send signal to scheduler stating termination
    kill(getppid(), SIGRTMIN+2);

    destroyClk(false);    
    return 0;
}
