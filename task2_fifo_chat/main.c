#include <fcntl.h>     // open, close, O_RDONLY, O_WRONLY
#include <stdio.h>     // fgets, printf, perror
#include <stdlib.h>    // exit
#include <string.h>    // strcmp, strlen
#include <sys/stat.h>  // mkfifo
#include <sys/wait.h>  // wait
#include <unistd.h>    // fork, read, write, unlink, sleep

#define FIFO_NAME "chat"  // название канала
#define BUFFER_SIZE 100   // размер буфера сообщения

int main() {
    // создание FIFO и обработка ошибок
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        perror("mkfifo");
        exit(1);
    }

    pid_t pid = fork();

    // выходим если не смогли создать дочерний процесс
    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {                // код дочернего процесса
        char buffer[BUFFER_SIZE];  // буфер для сообщений
        int fd;

        // Открытие FIFO для чтения, обработка ошибки
        fd = open(FIFO_NAME, O_RDONLY);
        if (fd == -1) {
            perror("open");
            exit(1);
        }

        while (1) {
            // читаем сообщение
            ssize_t bytesRead = read(fd, buffer, BUFFER_SIZE);

            buffer[bytesRead] = '\0';
            printf("Получено: %s\n", buffer);

            // завершаемся при получении команды exit
            if (strcmp(buffer, "exit") == 0) break;
        }

        close(fd);
    } else {  // код родительского процесса
        char message[BUFFER_SIZE];
        int fd;

        // открываем FIFO на запись, обрабатываем ошибку
        fd = open(FIFO_NAME, O_WRONLY);
        if (fd == -1) {
            perror("open");
            exit(1);
        }

        while (1) {
            printf("Введите сообщение: ");
            fgets(message, BUFFER_SIZE, stdin);
            message[strlen(message) - 1] = 0;  // удаляем символ новой

            // отправляем сообщение
            write(fd, message, strlen(message));

            // выходим после команды exit. важно делать это после отправки
            // сообщения чтобы дочерний процесс тоже завершился
            if (strcmp(message, "exit") == 0) break;

            // ждем некоторое время, чтобы дочерний процесс получил сообщение и
            // обработал его
            sleep(1);
        }

        close(fd);
        wait(NULL);  // ждем конца дочернего процесса

        // удаляем FIFO и обрабатываем ошибку
        if (unlink(FIFO_NAME) == -1) {
            perror("unlink failed");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}