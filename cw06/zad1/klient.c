#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include "server.h"
#include <signal.h>

int serverID;
int exitFlag = 0;
int queueID;

int main(int argc, char** argv){
    atexit(exitClient);
    signal(SIGUSR1, signalHandler);

    pid_t pid = getpid();
    queueID = msgget(IPC_PRIVATE, IPC_CREAT | 0660);
    if(queueID == -1){
        printf("Error in msgget\n");
        return 1;
    }

    char *path = getenv("HOME");
    key_t serverKey = ftok(path, PROJECT_ID);
    serverID = msgget(serverKey, 0);
    if(serverID == -1){
        printf("Error in msgget\n");
        return 1;
    }

    connect();

    while(exitFlag == 0){
        processCommand();
    }

    return 0;
}

void processCommand(){
    Message msg;
    Types command;
    char* input = malloc(sizeof(char) * 100);

    printf("Write one command: ");
    fgets(input, 100, stdin);
    char* buffor = strtok(input, " \n");

    if(strcmp(buffor, "ECHO") == 0) command = ECHO;
    else if(strcmp(buffor, "WERS") == 0) command = WERS;
    else if(strcmp(buffor, "TIME") == 0) command = TIME;
    else if(strcmp(buffor, "STOP") == 0) command = STOP;
    else if(strcmp(buffor, "FINISH") == 0) command = FINISH;
    else{
        printf("Wrong command\n");
        return;
    }

    buffor = strtok(NULL, "\n");
    msg.senderPid = getpid();
    msg.type = command;

    if(command == ECHO || command == WERS)
        strcpy(msg.mesg, buffor);

    sendMessageToServer(&msg);

    if(command == FINISH || command == STOP)
        exit(0);

    if(msgrcv(queueID, &msg, SIZE, 0, 0) == -1){
        printf("Message receive error\n");
        exit(0);
    }

    printf("Odebrana wiadomosc: %s\n", msg.mesg);
}

void signalHandler(int signum){
    printf("\nServer is closed. Ending work\n");
    exit(0);
}

void exitClient(){
    msgctl(queueID, IPC_RMID, NULL);
    printf("Klient with PID: %d finished the work\n", getpid());

    exit(0);
}

void connect(){
    Message msg;
    msg.type = START;
    msg.senderPid = getpid();
    sprintf(msg.mesg, "%d", queueID);
    sendMessageToServer(&msg);

    if(msgrcv(queueID, &msg, SIZE, 0, 0) == -1){
        printf("Message receive error\n");
        exit(0);
    }

    if(msg.type == STOP){
        printf("Cannot connect to server\n");
        exit(0);
    }

    int clientID = atoi(msg.mesg);
    printf("Client %d connected to server\n", clientID);
}

void sendMessageToServer(Message *msg){
    if(msgsnd(serverID, msg, SIZE, 0) == -1){
        printf("Error in msgsend\n");
        exit(0);
    }
}