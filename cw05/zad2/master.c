#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

double RE_MAX = 1.0;
double RE_MIN = -2.0;
double IM_MAX = 1.0;
double IM_MIN = -1.0;

int main(int argc, char** argv){
    if(argc != 3){
        printf("Niewlasciwa liczba argumentow!\n");
        return 1;
    }

    char* path = argv[1];
    int R = atoi(argv[2]);

    int T[R+1][R+1];

    mkfifo(path, 0666);

    int j;
    for(j = 0; j < 5; j++){
        pid_t pid = fork();
        if(pid == 0){
            execlp("./slave", "./slave", path, "500000", "200", (char*) NULL);
        }
    }

    sleep(1);

    FILE* fp = fopen(path, "r");
    if(fp == NULL){
        printf("Blad otwarcia\n");
        return 1;
    }

    int RangeRe = RE_MAX - RE_MIN;
    int RangeIM = IM_MAX - IM_MIN;

    char buf[100];
    while(fgets(buf, 100, fp) != NULL){
        double re, im;
        int iters;
        sscanf(buf, "%lf %lf %d\n", &re, &im, &iters);
        re = re - RE_MIN;
        im = im - IM_MIN;

        int reX = (int)((re/RangeRe)*R);
        int imY = (int)((im/RangeIM)*R);

        T[reX][imY] = iters;
    }

    close(fp);

    FILE* data = fopen("data", "w");
    int iter, iter2;
    for(iter = 0; iter < R; iter++)
        for(iter2 = 0; iter2 < R; iter2++)
            fprintf(data, "%d %d %d\n", iter2, iter, T[iter2][iter]);
    fclose(data);

    FILE* gnup = popen("gnuplot", "w");
    fputs("set view map\n", gnup);
    fprintf(gnup, "set xrange [0:%d]\n", R);
    fprintf(gnup, "set yrange [0:%d]\n", R);
    fputs("plot 'data' with image\n", gnup);

    fflush(gnup);
    getc(stdin);
    pclose(gnup);

    return 0;
}