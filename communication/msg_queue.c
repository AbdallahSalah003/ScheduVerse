#include "msg_queue.h"

void constructMsgQueue()
{
    key_t msg_key = ftok("keyfile", 'A');
    globalMsgQueueID = msgget(msg_key, 0666 | IPC_CREAT);

    if(msg_key == -1) 
    {
        perror("Error constructing Msg Queue");
        exit(-1);
    }
    printf("Msg Queue has been created successfully!");
}
void destroyMsgQueue()
{
    int check = msgctl(globalMsgQueueID, IPC_RMID, (void *)NULL);
    if(check == -1)
    {
        perror("Error Destroying Msg Queue");
        exit(-1);
    }
    printf("Msg Queue has been destroyed successfully!");
}
MsgBuff constructMsg(ProcessData process) 
{
    MsgBuff newMsg;
    newMsg.mtype = 12;
    newMsg.process = process;
    return newMsg;
}
bool sendMsg(MsgBuff msg)
{
    int check = msgsnd(globalMsgQueueID, &msg, sizeof(msg.process), !IPC_NOWAIT);
    if(check == -1)
    {
        perror("Error in sending msg");
        exit(-1);
    }
}
bool recvMsg(long msgType)
{
    
}
bool isMsgQueueEmpty()
{
    struct msqid_ds buf;

    if (msgctl(globalMsgQueueID, IPC_STAT, &buf) == -1) 
    {
        perror("Error in checking the Msg Queue");
        exit(-1); 
    }
    if (buf.msg_qnum == 0) 
    {
        return 1; 
    } 
    return 0; 
}