#ifndef LAB6_SERVER_H
#define LAB6_SERVER_H

#define MAX_CLIENTS 10
#define MAX_MESSAGE 100
#define QUEUE_SIZE 5

typedef enum Types{
    START = 1, ECHO, WERS, TIME, STOP, FINISH
} Types;

typedef struct Message{
    long type;
    pid_t senderPid;
    char mesg[MAX_MESSAGE];
}Message;

typedef struct Client{
    pid_t pid;
    mqd_t qid;
    int clientID;
}Client;

static const int PROJECT_ID = 20;
static const int SIZE = sizeof(Message);
static const char* SERVER_NAME = "/server100";

void startClient(Message *msg);
void doEcho(Message *msg);
void doWers(Message *msg);
void doTime(Message *msg);
void stopClient(Message *msg);
void finishWork(int endingProcess);
void sendMessage(mqd_t key, Message *msg);
void exitServer();
void receiveMessage();
long queueEmpty();
mqd_t getClientQID(pid_t clientID);

void sendMessageToServer(Message *msg);
void connect();
void makeTime(Message *msg);
void makeStop(Message *msg);
void makeFinish(Message *msg);
void exitClient();
void signalHandler(int signum);
void processCommand();

#endif //LAB6_SERVER_H
