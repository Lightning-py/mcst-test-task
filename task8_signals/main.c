#include <signal.h>  // signal, sigemptyset, sigaction
#include <stdio.h>   // printf
#include <stdlib.h>  // rand
#include <unistd.h>  // sleep

#define SIZE 1000

int sigint_received = 0;

int matrixA[SIZE][SIZE];
int matrixB[SIZE][SIZE];
int result[SIZE][SIZE];

#define USE_SIGACTION
// #define USE_SIGNAL

// функция для инициализации матриц рандомными числами
void initialize_matrices() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrixA[i][j] = rand() % 1000;
            matrixB[i][j] = rand() % 1000;
            result[i][j] = 0;
        }
    }
}

// функция для обработки sigint
void handle_sigint(int sig) {
    // когда получили сигнал устанавливаем счетчик в 1 и возвращаем обычную
    // реакцию на сигнал
    if (sigint_received == 0) {
        sigint_received = 1;
        signal(SIGINT, SIG_DFL);
    }
}

void multiply_matrices() {
    // обычное умножение матриц
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < SIZE; k++)
                result[i][j] += matrixA[i][k] * matrixB[k][j];

            sleep(1);

            // если получили сигнал выводим информацию
            if (sigint_received) {
                sigint_received = 0;
                printf("counters: line %d, column %d\n", i, j);
            }
        }
    }
}

int main() {
    initialize_matrices();

// устанаваливаем реакцию на сигнал
#ifdef USE_SIGNAL
    signal(SIGINT, handle_sigint);
#else
#ifdef USE_SIGACTION
    struct sigaction sig;

    sig.sa_handler = handle_sigint;  // устанавливаем функцию обработчика
    sigemptyset(&sig.sa_mask);  // очищаем маску сигналов
    sig.sa_flags = 0;  // устанаваливаем флаги в нуль

    // устанавливаем обработчик и обрабатываем ошибку
    if (sigaction(SIGINT, &sig, NULL) == -1) {
        perror("Signal handler error");

        return 1;
    }
#endif  // USE_SIGACTION
#endif

    // выполняем умножение
    multiply_matrices();

    return 0;
}