#include <stdio.h>     // printf, perror
#include <sys/stat.h>  // stat, S_IS...
#include <time.h>      // ctime

int main(int argc, char *argv[]) {
    if (argc != 2) {  // если больше или меньше двух аргументов - выходим
        printf("Invalid usage\n");
        return 1;
    }

    struct stat filestat;

    // Получение информации о файле и обработка ошибок
    if (stat(argv[1], &filestat) < 0) {
        perror("stat");
        return 1;
    }

    // Вывод информации о файле
    printf("File: %s\n", argv[1]);
    printf("Size: %lld bytes\n", (unsigned long long)filestat.st_size);
    printf("Permissions: %o\n", filestat.st_mode & 0777);
    printf("Last modified: %s", ctime(&filestat.st_mtime));
    printf("Last accessed: %s", ctime(&filestat.st_atime));
    printf("Last status change: %s", ctime(&filestat.st_ctime));

    // определение типа файла
    if (S_ISREG(filestat.st_mode)) {
        printf("Type: Regular file\n");
    } else if (S_ISDIR(filestat.st_mode)) {
        printf("Type: Directory\n");
    } else if (S_ISCHR(filestat.st_mode)) {
        printf("Type: Character device\n");
    } else if (S_ISBLK(filestat.st_mode)) {
        printf("Type: Block device\n");
    } else {
        printf("Type: Unknown\n");
    }

    return 0;
}