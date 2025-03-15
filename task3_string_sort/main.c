#include <stdio.h>

#include "reader.c"
#include "sorts.c"

int main(int argc, char** argv) {
    // проверяем количество аргументов
    if (argc != 4) {
        if (argc > 4) {  // если больше чем нужно - выводим лишние
            printf("Extra arguments: ");
            for (int i = 4; i < argc; ++i) printf("%s ", argv[i]);
            printf("\n");
        } else if (argc < 4) {  // если меньше чем нужно - сообщаем
            printf("Not enought arguments\n");
        }

        exit(1);  // выходим
    }

    // определяем метод сортировки
    int method = 0;
    if (!strcmp(argv[3], "quicksort")) {
        method = 1;
    } else if (!strcmp(argv[3], "mergesort")) {
        method = 2;
    } else if (!strcmp(argv[3], "bubblesort")) {
        method = 3;
    } else {  // если не знаем такой метод - выходим
        printf("Unknown method \"%s\"\n", argv[3]);
        exit(1);
    }

    // пытаемся открыть файл ввода, обрабатываем ошибку
    FILE* input = fopen(argv[1], "r");
    if (input == NULL) {
        perror("fopen");
        exit(1);
    }

    // пытаемся открыть файл вывода, обрабатываем ошибку
    FILE* output = fopen(argv[2], "w");
    if (output == NULL) {
        perror("fopen");
        exit(1);
    }

    char** strings;
    size_t size = 0;

    // читаем файл построчно, закрываем файл ввода
    int result = readFile(input, &strings, &size);
    fclose(input);

    if (!result) {  // если чтение файла завершилось удачно
        switch (method) {  // выбираем нужный метод сортировки, сортируем
            case 1:
                quickSort(strings, 0, size - 1);
                break;
            case 2:
                mergeSort(strings, 0, size - 1);
                break;
            case 3:
                bubbleSort(strings, size);
                break;
        }
    } else {  // если произошла ошибка во время чтения файла - выходим
        printf("Error while reading file %s\n", argv[1]);
        exit(1);
    }

    // выводим отсортированные строки в массив, закрываем файл вывода
    for (size_t i = 0; i < size; ++i) {
        fprintf(output, "%s\n", strings[i]);
    }

    fclose(output);

    return 0;
}