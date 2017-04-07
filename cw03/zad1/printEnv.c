#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    char* nazwaZmiennej;
    if(argc != 2){
        printf("Niewlasciwa liczba arugmentow!\n");
        return 1;
    }

    nazwaZmiennej = argv[1];
    char* wartoscZmiennej = getenv(nazwaZmiennej);
    if(wartoscZmiennej != NULL)
        printf("Wartosc zmiennej: %s wynosi: %s\n", nazwaZmiennej, wartoscZmiennej);
    else
        printf("Nie ma zmiennej o takiej nazwie: %s\n", nazwaZmiennej);
    return 0;
}