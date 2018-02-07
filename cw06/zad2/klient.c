#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include "server.h"

char queueName[NAME_MAX];
int exitFlag = 0;
mqd_t queueID;
mqd_t serverID;

int main(int argc, char** argv){
    atexit(exitClient);
    signal(SIGUSR1, signalHandler);

    struct mq_attr attr;
    attr.mq_msgsize = SIZE;
    attr.mq_maxmsg = QUEUE_SIZE;
    snprintf(queueName, NAME_MAX, "/client-%d", getpid());

    queueID = mq_open(queueName, O_CREAT | O_RDONLY, 0660, &attr);
    if(queueID == -1){
        printf("Error in msgget\n");
        return 1;
    }

    serverID = mq_open(SERVER_NAME, O_WRONLY);
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
    unsigned int priority = 1;

    if(command == ECHO || command == WERS)
        strcpy(msg.mesg, buffor);

    sendMessageToServer(&msg);

    if(command == FINISH || command == STOP)
        exit(0);

    if(mq_receive(queueID, (char*) &msg, SIZE, &priority) == -1){
        printf("Message receive error\n");
        exit(0);
    }

    printf("Received message: %s\n", msg.mesg);
}

void signalHandler(int signum){
    printf("\nServer is closed. Ending work\n");
    exit(0);
}

void exitClient(){
    if(mq_close(queueID) == -1)
        printf("Fail in closing\n");
    if(mq_unlink(queueName) == -1)
        printf("Fail in deleting\n");
    printf("Client with PID: %d finished the work\n", getpid());

    exit(0);
}

void connect(){
    unsigned int priority = 1;
    Message msg;
    msg.type = START;
    msg.senderPid = getpid();
    strcpy(msg.mesg, queueName);
    sendMessageToServer(&msg);

    if(mq_receive(queueID, (char*) &msg, SIZE, &priority) == -1){
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
    if(mq_send(serverID, (char*) msg, SIZE, 1) == -1){
        printf("Error in msgsend\n");
        exit(0);
    }
}