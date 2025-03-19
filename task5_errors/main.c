#include <errno.h>   // errno
#include <fcntl.h>   // open
#include <stdio.h>   // printf, perror
#include <string.h>  // strerror
#include <unistd.h>  // close

int main() {
    // открываем файл только на чтение
    int fd = open("file", O_RDONLY);

    // если не получилось открыть файл
    if (fd == -1) {
        // вывод через errno и strerror(errno)
        printf("Error opening file: errno = %d\n", errno);
        printf("Error message: %s\n", strerror(errno));

        // вывод через sys_nerr и sys_errlist устарел и не компилируется на моей
        // системе, поэтому я просто представлю код для работы в комментариях

        // if
        // (errno < sys_nerr) { // если значение errno меньше sys_nerror
        //     // выводим ошибку из массива sys_errlist
        //     printf("Error opening file: %s\n", sys_errlist[errno]);
        // } else { // если значение errno больше длины массива, значит ошибка
        // не лежит в sys_errlist
        //     printf("Error message: Unknown error\n");
        // }

        // вывод ошибки через perror
        perror("Error opening file");

        return 1;  // выход с ошибкой раньше времени
    }

    // если такой файл смог открыться, например, уже существовал
    // то нужно его закрыть
    printf("File opened successfully.\n");
    close(fd);

    return 0;  // выходим без ошибки
}