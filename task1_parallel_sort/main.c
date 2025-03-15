#include <stdio.h>

#include "sort.c"

int main() {
    int* array;
    size_t n = 0;

    char end = ' ';  // символ для проверки на ввод

    printf("Enter array size: ");
    int result = scanf("%lu%c", &n, &end);
    // вводим число и символ, чтобы удостовериться
    // что после числа нет допольнительных символов

    // проверяем введенные данные на корректность
    if (result != 2 || end != '\n') {
        printf("Invalid value n\n");
        exit(1);
    }

    array = calloc(n, sizeof(int));
    if (!array) {  // выходим в случае если память не аллоцирована
        printf("Not enought of memory to allocate array of size %lu\n", n);
        exit(1);
    }

    // заполняем массив рандомными значениями и выводим
    printf("Array: \n");
    for (size_t i = 0; i < n; i++) {
        array[i] = rand() % 1000;
        printf("%d ", array[i]);
    }
    printf("\n");

    ThreadData data = {array, 0, n - 1};
    merge_sort(&data);

    // выводим отсортированный массив
    printf("Sorted array: \n");
    for (size_t i = 0; i < n; i++) printf("%d ", array[i]);
    printf("\n");

    // освобождаем память
    free(array);

    return 0;
}