#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

void generate();
void sysShuffle();
void libShuffle();
void sysSort();
void libSort();

int main(int argc, char** argv){
    char* opcja = "";
    char* plik = "";
    char* dlugoscRekordu = "";
    char* liczbaRekordow = "";
    char* sysOrlib = "";

    srand(time(NULL));

    if(argc == 5){
        opcja = argv[1];
        plik = argv[2];
        liczbaRekordow = argv[3];
        dlugoscRekordu = argv[4];

        if(strcmp(opcja, "generate") == 0){
            generate(plik, dlugoscRekordu, liczbaRekordow);
            return 0;
        }
    }else if(argc == 6){
        sysOrlib = argv[1];
        opcja = argv[2];
        plik = argv[3];
        liczbaRekordow = argv[4];
        dlugoscRekordu = argv[5];
    }else{
        printf("Niewlasciwa liczba argmentow!\n");
        return 1;
    }

    if(strcmp(opcja, "shuffle") == 0){
        if(strcmp(sysOrlib, "sys") == 0)
            sysShuffle(plik, dlugoscRekordu, liczbaRekordow);
        else if(strcmp(sysOrlib, "lib") == 0)
            libShuffle(plik, dlugoscRekordu, liczbaRekordow);
    }else if(strcmp(opcja, "sort") == 0){
        if(strcmp(sysOrlib, "sys") == 0)
            sysSort(plik, dlugoscRekordu, liczbaRekordow);
        else if(strcmp(sysOrlib, "lib") == 0)
            libSort(plik, dlugoscRekordu, liczbaRekordow);
    }else
        printf("Niewlasciwy argument\n");

    return 0;
}

void generate(char* plik, char* dlugoscRekordu, char* liczbaRekordow){
    int dlugoscR = atoi(dlugoscRekordu);
    int liczbaR = atoi(liczbaRekordow);
    char bufor;

    int plikRandom = open("/dev/urandom", O_RDONLY);
    int plikDocelowy = open(plik, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR);

    int j;
    while(liczbaR > 0){
    	for(j = 0; j < dlugoscR; j++){
            read(plikRandom, &bufor, sizeof(bufor));
            write(plikDocelowy, &bufor, sizeof(bufor));
        }
        bufor = '\n';
    	write(plikDocelowy, &bufor, sizeof(bufor));
    	liczbaR--;
    }

    close(plikDocelowy);
    close(plikRandom);
}

void sysShuffle(char* plik, char* dlugoscRekordu, char* liczbaRekordow){
    int dlugoscR = atoi(dlugoscRekordu);
    int liczbaR = atoi(liczbaRekordow);
    dlugoscR++;
    char* rekord1 = malloc(sizeof(char) * dlugoscR);
    char* rekord2 = malloc(sizeof(char) * dlugoscR);

    int uchwytPliku = open(plik, O_RDWR);

    lseek(uchwytPliku, 0, SEEK_SET);
    
    int i, j;
    for(i = 0; i < liczbaR; i++){
        j = rand()%liczbaR;

        lseek(uchwytPliku, i * dlugoscR , SEEK_SET);
        read(uchwytPliku, rekord1, dlugoscR);
        lseek(uchwytPliku, j * dlugoscR, SEEK_SET);
        read(uchwytPliku, rekord2, dlugoscR);

        lseek(uchwytPliku, i * dlugoscR, SEEK_SET);
        write(uchwytPliku, rekord2, dlugoscR);
        lseek(uchwytPliku, j * dlugoscR , SEEK_SET);
        write(uchwytPliku, rekord1, dlugoscR);
    }
    close(uchwytPliku);
}

void libShuffle(char* plik, char* dlugoscRekordu, char* liczbaRekordow){
    int dlugoscR = atoi(dlugoscRekordu);
    int liczbaR = atoi(liczbaRekordow);
    dlugoscR++;
    char* rekord1 = malloc(sizeof(char) * dlugoscR);
    char* rekord2 = malloc(sizeof(char) * dlugoscR);

    FILE* uchwytPliku = fopen(plik, "r+");

    int i, j;
    for(i = 0; i < liczbaR; i++){
        j = rand()%liczbaR;

        fseek(uchwytPliku, i * dlugoscR, SEEK_SET);
        fread(rekord1, sizeof(char), dlugoscR, uchwytPliku);
        fseek(uchwytPliku, j * dlugoscR, SEEK_SET);
        fread(rekord2, sizeof(char), dlugoscR, uchwytPliku);

        fseek(uchwytPliku, i * dlugoscR, SEEK_SET);
        fwrite(rekord2, sizeof(char), dlugoscR, uchwytPliku);
        fseek(uchwytPliku, j * dlugoscR, SEEK_SET);
        fwrite(rekord1, sizeof(char), dlugoscR, uchwytPliku);
    }
    fclose(uchwytPliku);
}

void sysSort(char* plik, char* dlugoscRekordu, char* liczbaRekordow){
    int dlugoscR = atoi(dlugoscRekordu);
    int liczbaR = atoi(liczbaRekordow);
    dlugoscR++;
    char* rekord1 = malloc(sizeof(char) * dlugoscR);
    char* rekord2 = malloc(sizeof(char) * dlugoscR);

    int uchwytPliku = open(plik, O_RDWR);

    lseek(uchwytPliku, 0, SEEK_SET);

    int i, j;
    for(i = 0; i < liczbaR; i++){
        for(j = 0; j < liczbaR; j++){
            lseek(uchwytPliku, j * dlugoscR , SEEK_SET);
            read(uchwytPliku, rekord1, dlugoscR);
            lseek(uchwytPliku, (j+1) * dlugoscR, SEEK_SET);
            read(uchwytPliku, rekord2, dlugoscR);

            if(strcmp(rekord1, rekord2) > 0){
                lseek(uchwytPliku, j * dlugoscR, SEEK_SET);
                write(uchwytPliku, rekord2, dlugoscR);
                lseek(uchwytPliku, (j+1) * dlugoscR , SEEK_SET);
                write(uchwytPliku, rekord1, dlugoscR);
            }
        }
    }
    close(uchwytPliku);
}

void libSort(char* plik, char* dlugoscRekordu, char* liczbaRekordow){
    int dlugoscR = atoi(dlugoscRekordu);
    int liczbaR = atoi(liczbaRekordow);
    dlugoscR++;
    char* rekord1 = malloc(sizeof(char) * dlugoscR);
    char* rekord2 = malloc(sizeof(char) * dlugoscR);

    FILE* uchwytPliku = fopen(plik, "r+");

    int i, j;
    for(i = 0; i < liczbaR; i++){
        for(j = 0; j < liczbaR; j++){
            fseek(uchwytPliku, j * dlugoscR, SEEK_SET);
            fread(rekord1, sizeof(char), dlugoscR, uchwytPliku);
            fseek(uchwytPliku, (j+1) * dlugoscR, SEEK_SET);
            fread(rekord2, sizeof(char), dlugoscR, uchwytPliku);

            if(strcmp(rekord1, rekord2) > 0){
                fseek(uchwytPliku, j * dlugoscR, SEEK_SET);
                fwrite(rekord2, sizeof(char), dlugoscR, uchwytPliku);
                fseek(uchwytPliku, (j+1) * dlugoscR, SEEK_SET);
                fwrite(rekord1, sizeof(char), dlugoscR, uchwytPliku);
            }
        }
    }
    fclose(uchwytPliku);
}