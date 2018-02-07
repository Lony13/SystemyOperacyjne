#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include "server.h"
#include <time.h>
#include <ctype.h>
#include <signal.h>

int clients[MAX_CLIENTS][3]; //0 - pid, 1 - queueID, 2 - clientID
int numberOfClients = 0;
int clientID = 1;
int exitFlag = 0;
int serverID;
int endingProcess;

int main(int argc, char** argv){
    atexit(exitServer);

    char *path = getenv("HOME");
    key_t serverKey = ftok(path, PROJECT_ID);
    serverID = msgget(serverKey, IPC_CREAT | 0660);
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

    if((msgrcv(serverID, &msg, SIZE, 0, 0)) == -1){
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
    msgctl(serverID, IPC_RMID, NULL);
    printf("Server is closed\n");
    exit(0);
}

void startClient(Message *msg){
    pid_t pid = msg -> senderPid;
    int queueID = atoi(msg -> mesg);

    Message msg2;
    msg2.senderPid = getpid();
    if(numberOfClients == MAX_CLIENTS){
        msg2.type = STOP;
        printf("Server is full\n");
    } else{
        msg2.type = START;
        sprintf(msg2.mesg, "%d", clientID);
        clients[numberOfClients][0] = pid;
        clients[numberOfClients][1] = queueID;
        clients[numberOfClients][2] = clientID;
        printf("New client connected to server. Client PID: %d, ID: %d\n",
               clients[numberOfClients][0], clients[numberOfClients][2]);
        clientID++;
        numberOfClients++;
    }
    sendMessage(queueID, &msg2);
}

void doEcho(Message *msg){
    printf("Received Echo\n");
    int clientID = getClientID(msg -> senderPid);
    sendMessage(clientID, msg);
}

void doWers(Message *msg){
    printf("Received Wers\n");
    int clientID = getClientID(msg -> senderPid);
    int i;
    for(i = 0; i < strlen(msg -> mesg); i++)
        msg -> mesg[i] = toupper(msg -> mesg[i]);
    sendMessage(clientID, msg);
}

void doTime(Message *msg){
    printf("Received Time\n");
    int clientID = getClientID(msg -> senderPid);

    time_t curTime;
    time(&curTime);
    char* data = ctime(&curTime);
    strcpy(msg -> mesg, data);

    sendMessage(clientID, msg);
}

void stopClient(Message *msg){
    pid_t pid = msg -> senderPid;
    int i;
    for(i = 0; i < MAX_CLIENTS; i++)
        if(pid == clients[i][0])
            break;
    printf("Client with PID: %d Number: %d is deleted\n", clients[i][0], clients[i][2]);
    while(i < MAX_CLIENTS - 1){
        clients[i][0] = clients[i+1][0];
        clients[i][1] = clients[i+1][1];
        clients[i][2] = clients[i+1][2];
        i++;
    }
    numberOfClients--;
}

int getClientID(pid_t clientID){
    int i;
    for(i = 0; i < numberOfClients; i++)
        if(clientID == clients[i][0])
            return clients[i][1];
    return -1;
}

int queueEmpty(){
    struct msqid_ds stats;
    msgctl(serverID, IPC_STAT, &stats);
    return (stats.msg_qnum == 0);
}

void finishWork(int endingProcess){
    while(!queueEmpty())
        receiveMessage();

    int i;
    for(i = 0; i < numberOfClients; i++) {
        if(clients[i][0] != endingProcess) {
            printf("Sending finish message to: PID: %d\n", clients[i][0]);
            kill(clients[i][0], SIGUSR1);
        }
    }
}

void sendMessage(int key, Message *msg){
    if(msgsnd(key, msg, SIZE, 0) == -1){
        printf("Message send error\n");
        exit(1);
    }
}