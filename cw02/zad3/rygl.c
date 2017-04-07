#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

struct flock blokada;
int wybranyZnak, plik;
char bufor[100];

void trybInteraktywny();
void ustawRygielOdczytu();
void ustawRygielZapisu();
void wyswietlRygle();
void usunRygiel();
void odczytajZnak();
void zmienZnak();

int main(int argc, char const *argv[]){
        plik = open(argv[1], O_RDWR);
        (plik == -1) ? printf("Nie mozna otworzyc pliku\n") : trybInteraktywny();
        return 0;
}

void trybInteraktywny(){
        int wybranaOpcja;
        for(;;){
                blokada.l_len = 1;
                blokada.l_whence = SEEK_SET;

                printf("\n---- Tryb interaktywny: ----\n");
                printf("\n1. Ustaw rygiel na odczyt\n");
                printf("2. Ustaw rygiel na zapis\n");
                printf("3. Wyswietl zalozone rygle\n");
                printf("4. Usun rygiel\n");               
                printf("5. Odczytaj znak z pliky\n");
                printf("6. Zmien znak w pliku\n");
                printf("7. Wyjscie\n$: ");

                if ((fgets(bufor, sizeof(bufor), stdin)) && (sscanf(bufor, "%d", &wybranaOpcja) != 1)) break;

                switch(wybranaOpcja){
                        case 1: ustawRygielOdczytu(); break;
                        case 2: ustawRygielZapisu(); break;
                        case 3: wyswietlRygle(); break;
                        case 4: usunRygiel(); break;
                        case 5: odczytajZnak(); break;
                        case 6: zmienZnak(); break;
                        case 7: return;
                }
        }
}

void ustawRygielOdczytu(){
        printf("Wybierz znak:  ");

        if ((fgets(bufor, sizeof(bufor), stdin)) && (sscanf(bufor, "%d", &wybranyZnak) != 1)) return;

        blokada.l_type = F_RDLCK;
        blokada.l_start = wybranyZnak;
        
        if(fcntl(plik, F_SETLK, &blokada) != -1)
                printf("Rygiel odczytu zostal ustawiony na %d\n",wybranyZnak);
        else
                printf("Nie udalo ustawic sie rygla\n");
}

void ustawRygielZapisu(){
        printf("Wybierz znak:  ");
        if ((fgets(bufor, sizeof(bufor), stdin)) && (sscanf(bufor, "%d", &wybranyZnak) != 1)) return;
        
        blokada.l_type = F_WRLCK;
        blokada.l_start = wybranyZnak;

        if(fcntl(plik, F_SETLK, &blokada) != -1)
                printf("Rygiel zapisu zostal ustawiony na %d\n",wybranyZnak);
        else
                printf("Nie udalo ustawic sie rygla\n");

}

void wyswietlRygle(){
        int koniecPliku, obecnaPozycja = 0;
        koniecPliku = lseek(plik, 0, SEEK_END);
        lseek(plik, 0, SEEK_SET);

        do{
                blokada.l_type = F_WRLCK;
                blokada.l_start = obecnaPozycja;

                fcntl(plik, F_GETLK, &blokada);

                if(blokada.l_type != F_UNLCK)
                        printf("%s%d %s%d\n", "Blokada na pozycji: ", obecnaPozycja ,"proces: ", blokada.l_pid);
        }while(++obecnaPozycja <= koniecPliku);
}

void usunRygiel(){
        printf("Wybierz znak:  ");
        if ((fgets(bufor, sizeof(bufor), stdin)) && (sscanf(bufor, "%d", &wybranyZnak) != 1)) return;
        
        blokada.l_type = F_UNLCK;
        blokada.l_start = wybranyZnak;

        if(fcntl(plik, F_SETLK, &blokada) != -1)
                printf("Usunieto rygiel z znaku %d\n", wybranyZnak);
        else
                printf("Blad\n");
}

void odczytajZnak(){
        char znak;

        printf("Wybierz numer znaku ktory chcesz odczytac: ");
        if ((fgets(bufor, sizeof(bufor), stdin)) && (sscanf(bufor, "%d", &wybranyZnak) != 1)) return;
        
        lseek(plik, wybranyZnak, SEEK_SET);

        if(read(plik, &znak, 1) == -1)
                printf("Wystapil blad podczas odczytu z pliku");
        else
                printf("Odczytywany bajt to: %c\n", znak);
}

void zmienZnak(){
        int nrZnakuDoZmiany;
        char nowyZnak;

        printf("Wybierz numer znaku do zmiany: ");
        if ((fgets(bufor, sizeof(bufor), stdin)) && (sscanf(bufor, "%d", &nrZnakuDoZmiany) != 1)) return;
        
        printf("Wpisz nowy znak: ");
        if ((fgets(bufor, sizeof(bufor), stdin)) && (sscanf(bufor, "%c", &nowyZnak) != 1)) return;

        lseek(plik, nrZnakuDoZmiany, SEEK_SET);

        if(write(plik, &nowyZnak, 1) == -1)
                printf("Wystapil blad podczas zapisu do pliku");
        else
                printf("Znak zostal zmieniony\n");
}