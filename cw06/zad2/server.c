#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include "server.h"
#include <time.h>
#include <ctype.h>

Client clients[MAX_CLIENTS];
int numberOfClients = 0;
int clientID = 1;
int exitFlag = 0;
int endingProcess;
mqd_t serverID;

int main(int argc, char** argv){
    atexit(exitServer);

    struct mq_attr attr;
    attr.mq_msgsize = SIZE;
    attr.mq_maxmsg = QUEUE_SIZE;
    serverID = mq_open(SERVER_NAME, O_RDONLY | O_CREAT, 0666, &attr);
    if(serverID == -1){
        printf("Error in msgget\n");
        return 1;
    }
    printf("Server started\n");

    while(exitFlag == 0){
        receiveMessage();
    }

    finishWork(endingProcess);

    return 0;
}

void receiveMessage(){
    Message msg;
    msg.senderPid = getpid();

    unsigned int priority = 1;
    if((mq_receive(serverID, (char*) &msg, SIZE, &priority)) == -1){
        printf("Message receive error\n");
        exit(0);
    }

    if(msg.type == START)
        startClient(&msg);
    else if(msg.type == ECHO)
        doEcho(&msg);
    else if(msg.type == WERS)
        doWers(&msg);
    else if(msg.type == TIME)
        doTime(&msg);
    else if(msg.type == STOP)
        stopClient(&msg);
    else if(msg.type == FINISH){
        endingProcess = msg.senderPid;
        printf("Ending work\n");
        exitFlag = 1;
    } else {
        printf("Illegal command\n");
    }
}

void exitServer(){
    if(mq_close(serverID) == -1)
        printf("Fail in closing\n");
    if(mq_unlink(SERVER_NAME) == -1)
        printf("Fail in deleting\n");
    printf("Server is closed\n");
    exit(0);
}

void startClient(Message *msg){
    pid_t pid = msg -> senderPid;
    char* queueName = msg -> mesg;
    mqd_t qid;

    if((qid = mq_open(queueName, O_WRONLY)) == -1){
        printf("%d cannot open client queue\n", pid);
        exit(0);
    }

    Message msg2;
    msg2.senderPid = getpid();
    if(numberOfClients == MAX_CLIENTS){
        msg2.type = STOP;
        printf("Server is full\n");
    } else{
        msg2.type = START;
        sprintf(msg2.mesg, "%d", clientID);
        Client clientTmp;
        clientTmp.pid = pid;
        clientTmp.qid = qid;
        clientTmp.clientID = clientID;
        clients[numberOfClients] = clientTmp;
        printf("New client connected to server. Client PID: %d, ID: %d\n",
               clients[numberOfClients].pid, clients[numberOfClients].clientID);
        clientID++;
        numberOfClients++;
    }
    sendMessage(qid, &msg2);
}

void doEcho(Message *msg){
    printf("Received Echo\n");
    mqd_t clientQID = getClientQID(msg -> senderPid);
    sendMessage(clientQID, msg);
}

void doWers(Message *msg){
    printf("Received Wers\n");
    mqd_t clientQID = getClientQID(msg -> senderPid);
    int i;
    for(i = 0; i < strlen(msg -> mesg); i++)
        msg -> mesg[i] = toupper(msg -> mesg[i]);
    sendMessage(clientQID, msg);
}

void doTime(Message *msg){
    printf("Received Time\n");
    mqd_t clientQID = getClientQID(msg -> senderPid);

    time_t curTime;
    time(&curTime);
    char* data = ctime(&curTime);
    strcpy(msg -> mesg, data);

    sendMessage(clientQID, msg);
}

void stopClient(Message *msg){
    pid_t pid = msg -> senderPid;
    int i;
    for(i = 0; i < MAX_CLIENTS; i++)
        if(pid == clients[i].pid)
            break;
    printf("Client with PID: %d Number: %d is deleted\n", clients[i].pid, clients[i].clientID);
    while(i < MAX_CLIENTS - 1){
        clients[i] = clients[i+1];
        i++;
    }
    numberOfClients--;
}

mqd_t getClientQID(pid_t clientID){
    int i;
    for(i = 0; i < numberOfClients; i++)
        if(clientID == clients[i].pid) {
            return clients[i].qid;
        }
    return -1;
}

long queueEmpty(){
    struct mq_attr attr;
    if(mq_getattr(serverID, &attr) == -1){
        printf("Error in getattr\n");
        exit(0);
    }
    return (attr.mq_curmsgs == 0);
}

void finishWork(int endingProcess){
    while(!queueEmpty())
        receiveMessage();

    int i;
    for(i = 0; i < numberOfClients; i++) {
        if(clients[i].pid != endingProcess) {
            printf("Sending finish message to: PID: %d\n", clients[i].pid);
            kill(clients[i].pid, SIGUSR1);
        }
    }
}

void sendMessage(mqd_t qid, Message *msg){
    if(mq_send(qid, (char*) msg, SIZE, 0) == -1){
        printf("Message send error\n");
        exit(1);
    }
}