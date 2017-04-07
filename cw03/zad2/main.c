#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>

int main(int argc, char** argv){
    extern char** environ;
    struct rlimit limitCPU, limitAS;
    struct rusage rusageTime;
    struct timeval czas;
    char* nazwaPliku;
    long long ograniczenieCzas;
    long long ograniczeniePamiec;

    if(argc != 4){
        printf("Niewlasciwa liczba arugmentow!\n");
        return 1;
    }

    nazwaPliku = argv[1];
    ograniczenieCzas = atoi(argv[2]);
    ograniczeniePamiec = atoi(argv[3]);
    ograniczeniePamiec *= 1048576;

    limitCPU.rlim_max = ograniczenieCzas;
    limitCPU.rlim_cur = ograniczenieCzas;
    limitAS.rlim_max = ograniczeniePamiec;
    limitAS.rlim_cur = ograniczeniePamiec;

    FILE* fileHandler;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    fileHandler = fopen(nazwaPliku, "r");
    if(fileHandler == NULL)
        exit(EXIT_FAILURE);

    int flag = 0;
    while((read = getline(&line, &len, fileHandler)) != -1){
        if(line[0] == '#'){
            int i;
            for(i = 0; i < read; i++){
                if(line[i] == ' ') {
                    flag = i;
                    break;
                }
            }

            if(flag != 0){
                char* variableData[2];
                char* argument = strtok(line, "# \n");

                int j = 0;
                while(argument != NULL){
                    variableData[j++] = argument;
                    argument = strtok(NULL, " ");
                }

                setenv(variableData[0], variableData[1], 1);
            } else {
                char* variableName = strtok(line, "# \n");
                unsetenv(variableName);
            }
            flag = 0;
        } else{
            char* func;
            char* argument;

            int i, numberOfArgs = 0;
            for(i = 0; i < read; i++){
                if(line[i] == ' ')
                    numberOfArgs++;
            }

            if(numberOfArgs > 5){
                printf("Za duzo argumentow\n");
                return 1;
            }

            char* argumenty[numberOfArgs];
            int j = 0;

            func = strtok(line, "# \n");
            argument = strtok(NULL, " \n");
            while(argument != NULL){
                argumenty[j++] = argument;
                argument = strtok(NULL, " \n");
            }

            int status = 1;
            pid_t pid = fork();
            if(pid == 0){
                setrlimit(RLIMIT_CPU, &limitCPU);
                setrlimit(RLIMIT_AS, &limitAS);

                switch (numberOfArgs) {
                    case 0:
                        execlp(func, func, (char *) NULL);
                        break;
                    case 1:
                        execlp(func, func, argumenty[0], (char *) NULL);
                        break;
                    case 2:
                        execlp(func, func, argumenty[0], argumenty[1], (char *) NULL);
                        break;
                    case 3:
                        execlp(func, func, argumenty[0], argumenty[1], argumenty[2], (char *) NULL);
                        break;
                    case 4:
                        execlp(func, func, argumenty[0], argumenty[1], argumenty[2], argumenty[3], (char *) NULL);
                        break;
                    case 5:
                        execlp(func, func, argumenty[0], argumenty[1], argumenty[2], argumenty[3], argumenty[4],
                               (char *) NULL);
                        break;
                }
                exit(0);
            } else{
                wait4(pid, &status, 0, &rusageTime);

                czas = rusageTime.ru_utime;
                double zuzytyCzas = czas.tv_sec + czas.tv_usec / 1000000.0;
                printf("Zuzyty czas uzytkownika: %.4lfs\n", zuzytyCzas);
                czas = rusageTime.ru_stime;
                zuzytyCzas = czas.tv_sec + czas.tv_usec / 1000000.0;
                printf("Zuzyty czas systemowy: %.4lfs\n", zuzytyCzas);

                if(status != 0) {
                    printf("Blad w funkcji: %s\n", func);
                    return 1;
                }
            }
            printf("-----\n");
        }
    }

    free(line);
    return 0;
}