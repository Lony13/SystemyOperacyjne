#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <ftw.h>

int rozmair;

int ftwFun(const char *fpath, const struct stat *statptr, int tflag) {
    if(tflag == FTW_F && statptr -> st_size <= rozmiar) 
        char absolutePath[PATH_MAX + 1];
        realpath(fpath, absolutePath);
        printf("Sciezka do pliku: %s\n", absolutePath);

        printf("Rozmiar pliku: %d bajtow\n", (int) statptr->st_size);
        printf("Prawa dostepu: ");
        stat->st_mode & S_IRUSR ? printf("r") : printf("-");
        stat->st_mode & S_IWUSR ? printf("w") : printf("-");
        stat->st_mode & S_IXUSR ? printf("x") : printf("-");
        stat->st_mode & S_IRGRP ? printf("r") : printf("-");
        stat->st_mode & S_IWGRP ? printf("w") : printf("-");
        stat->st_mode & S_IXGRP ? printf("x") : printf("-");
        stat->st_mode & S_IROTH ? printf("r") : printf("-");
        stat->st_mode & S_IWOTH ? printf("w") : printf("-");
        stat->st_mode & S_IXOTH ? printf("x") : printf("-");
        printf("\n");

        time_t time = statptr->st_mtime;
        struct tm *dateTime = localtime(&time);
        char buff[20];

        strftime(buff, 20, "%Y-%m-%d %H:%M:%S", dateTime);
        printf("Ostatnia modyfikacja: %s\n\n", buff);
    }
    return 0;
}

int main(int argc, char **argv) {
    rozmiar = atoi(argv[2]);
    ftw(argv[1], ftwFun, 1);
    return 0;
}