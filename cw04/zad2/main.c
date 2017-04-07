#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

int licznik = 0;
int N, K;
union sigval sigVal;
int* pidy;
struct timeval tv1;
struct timeval tv2;

void wypiszDane(int sig, siginfo_t *info, void *context){
    int odebranePid[N];
    pid_t childPid = info -> si_pid;

    licznik++;
    printf("Otrzymalem SIGUSR1. Sygnal numer %d, Wyslal go proces o PID = %d\n", licznik, childPid);
    odebranePid[licznik-1] = childPid;

    if(licznik == K){
        int i;
        for(i = 0; i < K; i++){
            kill(odebranePid[i], SIGALRM);
        }
    }else if(licznik >= K){
        kill(info -> si_pid, SIGALRM);
    }
}

void wrocDoPracy(){
    return;
}

void odbierzSygnal(int sig, siginfo_t *info, void *context){
    pid_t childPid = info -> si_pid;
    int signalNumber = info -> si_signo;
    sigval_t value = info -> si_value;

    printf("Proces %d zakonczyl prace. Time: %d nano sekund Signal Number: %d\n", childPid, value, signalNumber);
}

void zakonczProgram(){
    int i;
    for(i = 0; i < N; i++){
        kill(SIGINT, pidy[i]);
    }
    printf("Otrzymalem SIGINT wiec koncze program\n");
    exit(0);
}

int main(int argc, char** argv) {
    if(argc != 3){
        printf("Niewlasciwa liczba argumetow\n");
        return 1;
    }

    srand(time(NULL) ^ (getpid()<<16));

    N = atoi(argv[1]);
    K = atoi(argv[2]);

    struct sigaction act;
    act.sa_sigaction = &wypiszDane;
    act.sa_flags = SA_SIGINFO;

    struct sigaction actRT;
    actRT.sa_sigaction = &odbierzSygnal;
    actRT.sa_flags = SA_SIGINFO;

    int x;
    pid_t pid;
    pidy = malloc(sizeof(int)*N);

    for(x = 0; x < N; x++) {
        pid = fork();
        pidy[x] = pid;

        if (pid == 0) {
            sleep(1);

            srand(time(NULL) ^ (getpid()<<16));
            sleep(rand()%10);

            kill(getppid(), SIGUSR1);
            gettimeofday(&tv1, NULL);


            signal(SIGALRM, &wrocDoPracy);


            pause();


            srand(time(NULL) ^ (getpid()<<16));
            int number = 1 + rand()%32;

            gettimeofday(&tv2, NULL);

            sigVal.sival_int = (tv2.tv_sec*1000000000 + tv2.tv_usec) - (tv1.tv_sec * 1000000000 + tv1.tv_usec);
            sigqueue(getppid(), SIGRTMIN+number, sigVal);
            exit(0);
        } else {
            signal(SIGINT, &zakonczProgram);
            sigaction(SIGUSR1, &act, NULL);
            sigaction(SIGRTMIN, &actRT, NULL);
            sigaction(SIGRTMIN+1, &actRT, NULL);
            sigaction(SIGRTMIN+2, &actRT, NULL);
            sigaction(SIGRTMIN+3, &actRT, NULL);
            sigaction(SIGRTMIN+4, &actRT, NULL);
            sigaction(SIGRTMIN+5, &actRT, NULL);
            sigaction(SIGRTMIN+6, &actRT, NULL);
            sigaction(SIGRTMIN+7, &actRT, NULL);
            sigaction(SIGRTMIN+8, &actRT, NULL);
            sigaction(SIGRTMIN+9, &actRT, NULL);
            sigaction(SIGRTMIN+10, &actRT, NULL);
            sigaction(SIGRTMIN+11, &actRT, NULL);
            sigaction(SIGRTMIN+12, &actRT, NULL);
            sigaction(SIGRTMIN+13, &actRT, NULL);
            sigaction(SIGRTMIN+14, &actRT, NULL);
            sigaction(SIGRTMIN+15, &actRT, NULL);
            sigaction(SIGRTMIN+16, &actRT, NULL);
            sigaction(SIGRTMIN+17, &actRT, NULL);
            sigaction(SIGRTMIN+18, &actRT, NULL);
            sigaction(SIGRTMIN+19, &actRT, NULL);
            sigaction(SIGRTMIN+20, &actRT, NULL);
            sigaction(SIGRTMIN+21, &actRT, NULL);
            sigaction(SIGRTMIN+22, &actRT, NULL);
            sigaction(SIGRTMIN+23, &actRT, NULL);
            sigaction(SIGRTMIN+24, &actRT, NULL);
            sigaction(SIGRTMIN+25, &actRT, NULL);
            sigaction(SIGRTMIN+26, &actRT, NULL);
            sigaction(SIGRTMIN+27, &actRT, NULL);
            sigaction(SIGRTMIN+28, &actRT, NULL);
            sigaction(SIGRTMIN+29, &actRT, NULL);
            sigaction(SIGRTMIN+30, &actRT, NULL);
            sigaction(SIGRTMIN+31, &actRT, NULL);
            sigaction(SIGRTMIN+32, &actRT, NULL);
        }
    }

    for(;;){}
}