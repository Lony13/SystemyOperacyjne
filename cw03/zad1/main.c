#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    extern char** environ;
    char* nazwaPliku;

    if(argc != 2){
        printf("Niewlasciwa liczba arugmentow!\n");
        return 1;
    }

    nazwaPliku = argv[1];

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
                wait(&status);
                if(status != 0) {
                    printf("Blad w funkcji: %s\n", func);
                    return 1;
                }
            }
        }
    }

    free(line);
    return 0;
}