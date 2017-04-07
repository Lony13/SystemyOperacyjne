#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int licznikSygnalowChild = 0;
int licznikSygnalowParent = 0;
int licznikSygnalowWyslanych = 0;
pid_t parentPid, pid;
int Type;
union sigval sigVal;

void odbierzSygnalC(){
    printf("Dostalem sygnal - potomek. Sygnal numer: %d\n", ++licznikSygnalowChild);
    if(Type == 1) kill(parentPid, SIGUSR1);
    if(Type == 2) sigqueue(parentPid, SIGUSR1, sigVal);
    if(Type == 3) kill(parentPid, SIGRTMIN+1);
}

void zakonczPraceC(){
    printf("\nKoncze prace - potomek\n");
    printf("Odebralem %d sygnalow\n", licznikSygnalowChild);
    if(Type == 1) kill(parentPid, SIGUSR2);
    if(Type == 2) sigqueue(parentPid, SIGUSR2, sigVal);
    if(Type == 3) kill(parentPid, SIGRTMIN+2);
    exit(0);
}

void odbierzSygnalR(){
    printf("Dostalem sygnal - rodzic. Sygnal numer: %d\n", ++licznikSygnalowParent);
}

void zakonczPraceR(){
    printf("\nKoncze prace - rodzic\n");
    printf("Wyslalem %d sygnalow\n", licznikSygnalowWyslanych);
    printf("Odebralem %d sygnalow\n", licznikSygnalowParent);
    exit(0);
}

void zakonczProgram(){
    if(Type == 1) kill(pid, SIGUSR2);
    if(Type == 2) sigqueue(pid, SIGUSR2, sigVal);
    if(Type == 3) kill(pid, SIGRTMIN+2);
    printf("Dostalem SIGINT wiec koncze prace\n");
    exit(0);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Niewlasciwa liczba argumetow\n");
        return 1;
    }

    int L = atoi(argv[1]);
    Type = atoi(argv[2]);

    int i;
    parentPid = getpid();
    pid = fork();

    if(Type == 1) {
        if (pid == 0) {
            signal(SIGUSR1, &odbierzSygnalC);
            signal(SIGUSR2, &zakonczPraceC);
        } else {
            signal(SIGINT, &zakonczProgram);
            signal(SIGUSR1, &odbierzSygnalR);
            signal(SIGUSR2, &zakonczPraceR);

            sleep(1);

            for (i = 0; i < L; i++) {
                licznikSygnalowWyslanych++;
                kill(pid, SIGUSR1);
                sleep(1);
            }
            kill(pid, SIGUSR2);
        }
    } else if(Type == 2){
        if(pid == 0){
            signal(SIGUSR1, &odbierzSygnalC);
            signal(SIGUSR2, &zakonczPraceC);
        } else{
            signal(SIGINT, &zakonczProgram);
            signal(SIGUSR1, &odbierzSygnalR);
            signal(SIGUSR2, &zakonczPraceR);

            sleep(1);

            for(i = 0; i < L; i++){
                licznikSygnalowWyslanych++;
                sigqueue(pid, SIGUSR1, sigVal);
                sleep(1);
            }
            sigqueue(pid, SIGUSR2, sigVal);
        }
    } else if(Type == 3){
        if(pid == 0){
            signal(SIGRTMIN+1, &odbierzSygnalC);
            signal(SIGRTMIN+2, &zakonczPraceC);
        } else{
            signal(SIGINT, &zakonczProgram);
            signal(SIGRTMIN+1, &odbierzSygnalR);
            signal(SIGRTMIN+2, &zakonczPraceR);

            sleep(1);

            for(i = 0; i < L; i++){
                licznikSygnalowWyslanych++;
                kill(pid, SIGRTMIN+1);
                sleep(1);
            }
            kill(pid, SIGRTMIN+2);
        }

    } else{
        printf("Niepoprawna wartosc argumentu Type\n");
        printf("Argument Type musi byc rowny 1,2 lub 3\n");
    }

    for(;;){}
}