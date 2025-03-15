#include <stdlib.h>
#include <string.h>

// функция для чтения из файла
// одной строки неограниченного размера
// принимает на вход файл и указатель на строку,
// в которую будет положен результат
int readString(FILE* file, char** res) {
    size_t size = 128;  // изначальный размер буфера
    size_t len_str = 0;  // изначальная длина читаемой строки

    char* buffer = (char*)calloc(size, sizeof(char));  // буфер
    char c = fgetc(file);

    if (!buffer) {  // если память под буфер не может выделиться - выходим
        return -1;
    }

    // пока строка или файл не заканчиваются - читаем
    // по одному символу и записываем в буфер
    while (c != '\n' && c != EOF) {
        buffer[len_str] = c;
        len_str++;

        // если на следующий символ места не хватает
        // увеличиваем объем массива вдвое
        if (size <= len_str) {
            size *= 2;
            char* new_buffer = realloc(buffer, size);

            if (!new_buffer) {
                free(buffer);
                return -1;
            }

            buffer = new_buffer;
        }

        c = fgetc(file);
    }

    // делаем строку нуль-терминированной
    buffer[len_str] = '\0';

    // записываем результат
    *res = buffer;

    return 0;
}

// функция для построчного чтения файла неограниченного размера
// на вход принимает файл, указатель на массив строк
// в который будут положены строки файла и указатель на переменную
// в которую будут положен размер полученного массива строк
int readFile(FILE* file, char*** res, size_t* size_) {
    int size = 32;  // изначальный размер массива строк

    char** result = (char**)calloc(size, sizeof(char*));

    if (!result) {  // если память под массив не выделилась - выходим
        return -1;
    }

    int i = 0;  // счетчик количества строк
    while (!feof(file)) {  // пока файл не закончился - читаем строки
        char* str;

        if (readString(file, &str) != -1) {  // если смогли считать строку
            result[i] = str;
            i++;

            if (i >= size) {  // увеличиваем размер массива вдвое если его мало
                size *= 2;
                char** new_result = realloc(result, size * sizeof(char*));

                if (!new_result) {  // если не можем выделить память - выходим
                    for (int j = 0; j < i; ++j) free(new_result[j]);
                    free(result);
                    return -1;
                }

                result = new_result;
            }
        } else {  // если не смогли считать строку - выходим
            for (int j = 0; j < i; ++j) free(result[j]);
            free(result);
            return -1;
        }
    }

    // записываем результаты
    *res = result;
    *size_ = i;

    return 0;
}