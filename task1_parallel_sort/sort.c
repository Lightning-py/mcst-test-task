#pragma once

#include <pthread.h>
#include <stdlib.h>
#include <string.h>  // memcpy

// структура для передачи данных в поток
typedef struct {
    int *array;
    int left;
    int right;
} ThreadData;

// функция слияния для сортировки слиянием
void merge(int *array, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = (int *)calloc(n1, sizeof(int));
    int *R = (int *)calloc(n2, sizeof(int));

    memcpy(L, array + left, n1 * sizeof(int));
    memcpy(R, array + (mid + 1), n2 * sizeof(int));

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            array[k] = L[i];
            i++;
        } else {
            array[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        array[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        array[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void *merge_sort(void *arg) {
    // получаем аргументы
    ThreadData *data = (ThreadData *)arg;

    int left = data->left;
    int right = data->right;
    int mid;

    if (left < right) {
        mid = left + (right - left) / 2;

        // делим массив пополам, создаем два потока для сортировки и дожидаемся
        // завершения обоих потоков
        ThreadData leftData = {data->array, left, mid};
        ThreadData rightData = {data->array, mid + 1, right};

        pthread_t leftThread, rightThread;

        pthread_create(&leftThread, NULL, merge_sort, &leftData);
        pthread_create(&rightThread, NULL, merge_sort, &rightData);

        pthread_join(leftThread, NULL);
        pthread_join(rightThread, NULL);

        // сливаем результаты
        merge(data->array, left, mid, right);
    }

    return NULL;
}
