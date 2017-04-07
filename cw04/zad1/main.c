#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

//SIGTSTP - ctrl + z
//SIGINT - DELETE, Ctrl + c

int flaga = 0; //0 - normalnie, -1 - odwrocowa kolejnosc

void zakonczProgram(){
    printf("Odebrano sygnał SIGINT\n");
    exit(0);
}

void zmienKolejnosc(){
    if(flaga == 0)
        flaga = -1;
    else
        flaga = 0;
}

void wypiszAlfabet(){
    char litera = 65;
    struct sigaction act;
    act.sa_handler = zmienKolejnosc;

    for(;;){
        signal(SIGINT, zakonczProgram);

        sigaction(SIGTSTP, &act, 0);

        if(litera == 64)
            litera = 90;
        if(litera == 91)
            litera = 65;

        if(flaga == 0) {
            printf("%c\n", litera);
            litera += 1;
            sleep(1);
        } else{
            printf("%c\n", litera);
            litera -= 1;
            sleep(1);
        }
    }
}

int main() {
    wypiszAlfabet();

    return 0;
}
