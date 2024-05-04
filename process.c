#include "headers.h"

int shmRid;
key_t remaining_key;
int remainingtime;
int *remainingTime;
int time;
int prvtime;
int quantum;
int ret = 0;
int target = 0;
void timit(int signum)
{
    prvtime = getClk();
    time=getClk();
    ret = 1;
    remainingtime = *remainingTime;
    target = remainingtime - quantum;
    if (target < 0)
        target = 0;
    printf("\n\nTHIS PROCESS SHOULD CONTINUE NOW!!! WITH REMAINING TIME = %d, it should run till it reaches %d\n\n", *remainingTime,target);
    signal(SIGUSR1, timit);
}

int main(int agrc, char *argv[])
{
    signal(SIGUSR1, timit);
    remaining_key = ftok("remainingkey", 's');
    shmRid = shmget(remaining_key, 4096, IPC_CREAT | 0666);
    remainingTime = (int *)shmat(shmRid, (void *)0, 0);
    initClk();
    remainingtime = atoi(argv[1]);

    int ind = atoi(argv[2]);
    quantum = atoi(argv[3]);
    printf("process %d started running now now now\n",ind);
    while (remainingtime)
    {
        prvtime = getClk();
        time = getClk();
        printf("\nBEFORE CHANGING:: current clk is : %d, time: %d, prvtime: %d\n", getClk(), time, prvtime);
        while (prvtime == time)
        {
            time = getClk();
        }
        if(time<=prvtime) continue;
        printf("\nAFTER CHANGING::  current clk is : %d, time: %d, prvtime: %d\n", getClk(), time, prvtime);
        if(time<=prvtime) continue;
        prvtime = time;
        if(time<prvtime) continue;
        remainingtime--;
        (*remainingTime) = remainingtime;
        if(remainingtime==target&&target!=0 && quantum) {
            printf("waiting for the scheduler to suspend me!");
            sleep(1);
        }
        if (remainingtime <= 3)
            printf("\nprocess 2 realistically has %d left now", remainingtime);
        //  printf("Process %d has consumed sec and it's remaing time: %d\n",ind , remainingtime);
        if (remainingtime <= 0)
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
    kill(getppid(), SIGRTMIN + 2);
    shmdt(remainingTime);
    destroyClk(0);
    return 0;
}