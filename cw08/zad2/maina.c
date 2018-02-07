#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <pthread.h>
#include <syscall.h>
#include <signal.h>

#define RECORD_SIZE 1024

typedef struct threadData{
    pthread_t* threadTab;
}thread_data_t;

int file;
int mode;
int sigNum;
int recordsToRead;
int numberOfThreads;
char* keyWord;
pthread_mutex_t mutex;

void* readFile(void* arg);

void sigHandler(int signum){
    printf("PID: %d, TID: %d, SIGNUM: %d\n", getpid(), pthread_self(), sigNum);
}

int main(int argc, char** argv) {
    if(argc != 7){
        printf("Niewlasciwa liczba argumentow\n");
        exit(0);
    }

    numberOfThreads = atoi(argv[1]);
    char* fileName = argv[2];
    recordsToRead = atoi(argv[3]);
    keyWord = argv[4];
    mode = atoi(argv[5]);     //identyfikator modyfikacji
    sigNum = atoi(argv[6]);   //identyfikator sygnalu, 30 -> SIGUSR1, 15 -> SIGTERM, 9 -> SIGKILL, 19 -> SIGSTOP

    file = open(fileName, O_RDONLY);
    if(file == -1){
        printf("Blad podczas otwarcia pliku\n");
        exit(0);
    }

    if(pthread_mutex_init(&mutex, NULL) != 0){
        printf("Blad podczas inicjalizacji mutexu\n");
        exit(0);
    }

    thread_data_t data;
    data.threadTab = malloc(sizeof(pthread_t) * numberOfThreads);

    pthread_t threadID;
    int i;
    for(i = 0; i < numberOfThreads; i++){
        pthread_create(&threadID, NULL, &readFile, &data);
        data.threadTab[i] = threadID;
    }

    //1 -> wyslanie sygnalu do procesu bez maskowania go
    //2 -> wyslanie sygnalu do procesu majac zamaskowany w glownym
    //3 -> wyslanie sygnalu do procesu gdy wszystkie watki maja procedure obslugi go
    //4 -> wyslanie sygnalu do watku gdy ma go zamaskowany
    //5 -> wyslanie sygnalu do watku gdy ma procedure obslugi
    if(mode == 1){
        kill(getpid(), sigNum);
    } else if(mode == 2){
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, sigNum);
        sigprocmask(SIG_BLOCK, &set, NULL);
        kill(getpid(), sigNum);
    } else if(mode == 3){
        signal(sigNum, sigHandler);
        kill(getpid(), sigNum);
    } else if(mode == 4 || mode == 5){
        printf("Wysylam sygnal %d do watku %d\n", sigNum, (int)data.threadTab[numberOfThreads-1]);
        pthread_kill(data.threadTab[numberOfThreads-1], sigNum);
    }

    for(i = 0; i < numberOfThreads; i++)
        pthread_join(data.threadTab[i], NULL);
    printf("Koniec macierzystego\n");

    close(file);
    pthread_mutex_destroy(&mutex);

    return 0;
}

void* readFile(void *data){
    if(mode == 3 || mode == 5){
        signal(sigNum, sigHandler);
    } else if(mode == 4){
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, sigNum);
        sigprocmask(SIG_BLOCK, &set, NULL);
    }

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    thread_data_t* thData = data;
    char bufor[RECORD_SIZE];
    char recordID[11];
    recordID[10] = '\0';
    int i, j;

    while(1) {
        pthread_mutex_lock(&mutex);
        for(i = 0; i < recordsToRead; i++) {
            if (read(file, &bufor, sizeof(bufor)) == 0) {
                printf("Nic nie bylo do przeczytania %li\n", syscall(SYS_gettid));
                pthread_mutex_unlock(&mutex);
                return NULL;
            }

            if (strstr(bufor, keyWord) != NULL) {
                for(j = 0; j < 10; j++)
                    recordID[j] = bufor[j];
                printf("Watek %li znalazl kluczowe slowo w rekordzie o id: %s\n", syscall(SYS_gettid), recordID);
                for(j = 0; j < numberOfThreads; j++) {
                    if (pthread_self() != thData->threadTab[j]) {
                        pthread_cancel(thData->threadTab[j]);
                    }
                }
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
        }
        pthread_mutex_unlock(&mutex);
        usleep(1);
    }
    return NULL;
}