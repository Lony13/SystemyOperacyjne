#ifndef LAB6_SERVER_H
#define LAB6_SERVER_H

#define MAX_CLIENTS 10
#define MAX_MESSAGE 100

typedef enum Types{
    START = 1, ECHO, WERS, TIME, STOP, FINISH
} Types;

typedef struct Message{
    long type;
    pid_t senderPid;
    char mesg[MAX_MESSAGE];
}Message;

static const int PROJECT_ID = 20;
static const int SIZE = sizeof(Message) - sizeof(long);

void startClient(Message *msg);
void doEcho(Message *msg);
void doWers(Message *msg);
void doTime(Message *msg);
void stopClient(Message *msg);
void finishWork(int endingProcess);
void sendMessage(int key, Message *msg);
void exitServer();
void receiveMessage();
int getClientID(pid_t clientID);
int queueEmpty();

void sendMessageToServer(Message *msg);
void connect();
void makeTime(Message *msg);
void makeStop(Message *msg);
void makeFinish(Message *msg);
void exitClient();
void signalHandler(int signum);
void processCommand();


#endif //LAB6_SERVER_H
