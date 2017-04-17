#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* programs[20][5];

void execute(int p, int args){
    switch(args){
        case 1:
            execlp(programs[p][0], programs[p][0], (char*) NULL); break;
        case 2:
            execlp(programs[p][0], programs[p][0], programs[p][1], (char*) NULL); break;
        case 3:
            execlp(programs[p][0], programs[p][0], programs[p][1], programs[p][2],(char*) NULL); break;
        case 4:
            execlp(programs[p][0], programs[p][0], programs[p][1], programs[p][2], programs[p][3], (char*) NULL);
            break;
        case 5:
            execlp(programs[p][0], programs[p][0], programs[p][1], programs[p][2], programs[p][3], programs[p][4],
                   (char*) NULL); break;
        case 6:
            execlp(programs[p][0], programs[p][0], programs[p][1], programs[p][2], programs[p][3], programs[p][4],
                   programs[p][5],(char*) NULL); break;
    }
}

int main(int argc, char** argv) {
    char* input = malloc(sizeof(char) * 500);
    int countArgs[20];
    int numberOfPrograms = 0;
    int numberOfArguments = 0;

    int x;
    for(x = 0; x < 20; x++)
        countArgs[x] = 0;

    fgets(input, 500, stdin);

    char * buffor = strtok(input, " \n");
    do{
        if(strcmp(buffor, "|") == 0){
            countArgs[numberOfPrograms++] = numberOfArguments;
            numberOfArguments = 0;
        }else
            programs[numberOfPrograms][numberOfArguments++] = buffor;
        buffor = strtok(NULL, " \n");
    } while(buffor != NULL);
    countArgs[numberOfPrograms] = numberOfArguments;
    numberOfPrograms++;
    programs[numberOfPrograms][0] = NULL;

    int fd[20][2];
    pipe(fd[0]);
    pid_t pid = fork();

    if(pid == 0){
        close(fd[0][0]);
        dup2(fd[0][1], STDOUT_FILENO);
        execute(0, countArgs[0]);
    }else{
        int i;
        for(i = 1; i < numberOfPrograms - 1; i++){
            close(fd[i-1][1]);
            dup2(fd[i-1][0], STDIN_FILENO);

            pipe(fd[i]);
            pid_t pid2 = fork();
            if(pid2 == 0){
                close(fd[i][0]);
                dup2(fd[i][1], STDOUT_FILENO);
                execute(i, countArgs[i]);
            }
        }
    }

    close(fd[numberOfPrograms-2][1]);
    dup2(fd[numberOfPrograms-2][0], STDIN_FILENO);
    execute(numberOfPrograms-1, countArgs[numberOfPrograms-1]);

    return 0;
}