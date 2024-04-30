#pragma once
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>

key_t key_sem;
int sem_id;

union Semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
    void *__pad;
};
int constructSemaphore() {
    union  Semun semun;
    key_sem = ftok("keyfile", 'P');
    sem_id = semget(key_sem, 1, 0666 | IPC_CREAT);
    if(sem_id == -1)
    {
        perror("Error in create sem");
        return;
    }
    semun.val = 1;
    if(semctl(sem_id, 0, SETVAL, semun) == -1)
    {
        perror("Error in semctl");
    }
    return sem_id;
}
int getSemaphore() {
    key_t sem = ftok("keyfile", 'P');
    int id = semget(sem, 1, 0666);
    return id;
}
void down(int sem) {
    struct sembuf p_op;

    p_op.sem_num = 0;
    p_op.sem_op = -1;
    p_op.sem_flg = IPC_NOWAIT;

    if(semop(sem, &p_op, 1) == -1)
    {
        perror("Error in down()");
    }
}
void up(int sem) {
    struct sembuf v_op;

    v_op.sem_num = 0; // indicates sem index as we set semnum = 1
    v_op.sem_op = 1;
    v_op.sem_flg = IPC_NOWAIT;

    if(semop(sem, &v_op, 1) == -1)
    {
        perror("Error in up()");
    }
}
void deleteSemaphore(int semid) {
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("Error deleting semaphore");
    }
}