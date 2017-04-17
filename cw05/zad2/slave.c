#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <complex.h>

double RE_MAX = 1.0;
double RE_MIN = -2.0;
double IM_MAX = 1.0;
double IM_MIN = -1.0;

double complex newPoint(){
    double x = (double)rand()/RAND_MAX * (RE_MAX - RE_MIN) + RE_MIN;
    double y = (double)rand()/RAND_MAX * (IM_MAX - IM_MIN) + IM_MIN;
    return x + y * I;
}

int main(int argc, char** argv){
    if(argc != 4){
        printf("Niewlasciwa liczba argumentow!\n");
        return 1;
    }

    char* path = argv[1];
    int N = atoi(argv[2]);  //liczba punktow
    int K = atoi(argv[3]);  //liczba iteracji
    srand(time(NULL));

    int fd = open(path, O_WRONLY);
    if(fd < 0){
        printf("Blad otwarcia potoku\n");
        return 1;
    }

    int i, iters = 0;
    double complex tmp;
    char buffer[100];
    for(i = 0; i < N; i++){
        double complex point = newPoint();
        tmp = point;
        iters = 0;
        while(cabs(tmp) < 2.0){
            if(++iters == K) break;
            tmp = tmp * tmp + point;
        }
        sprintf(buffer, "%.3f %.3f %d\n", creal(point), cimag(point), iters);
        write(fd, buffer, strlen(buffer));
    }

    return 0;
}