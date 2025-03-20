#include <stdio.h>     // perror, fopen, fread, fwrite, fclose
#include <sys/wait.h>  // wait
#include <unistd.h>    // fork

#define BUFFER_SIZE 1024

// функция для копирования нужого файла в другой
int copy_file(FILE *input_file, const char *output_file) {
    // открываем файл, обрабатываем ошибку
    FILE *output_file_ptr = fopen(output_file, "w");
    if (output_file_ptr == NULL) {
        perror("Error opening output file");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    // копируем из файла в другой
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), input_file)) > 0) {
        fwrite(buffer, 1, bytes_read, output_file_ptr);
    }

    // закрываем вывода
    fclose(output_file_ptr);
    return 0;
}

// функция для вывода файла на экран
int print_file(const char *file_name) {
    // открываем файл, обрабатываем ошибку
    char buffer[BUFFER_SIZE];
    FILE *file = fopen(file_name, "r");

    if (file == NULL) {
        perror("Error opening input file");
        return 1;
    }

    // читаем из файла и выводим на экран
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    // закрываем файл
    fclose(file);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {  // если аргумента не два - выходим
        printf("Invalid usage\n");
        return 1;
    }

    // открываем файл ввода, открываем ошибку
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // вызываем fork, обрабатываем ошибку
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        fclose(input_file);
        return 1;
    }

    if (pid == 0) {  // код дочернего процесса
        char *child_output_file = "child.txt";  // пишем в файл child.txt

        // пытаемся скопировать файл
        int result = copy_file(input_file, child_output_file);
        fclose(input_file);

        // если файл скопировать не получилось - выходим
        if (result != 0) return 1;

        // пытаемся вывести файл
        result = print_file(child_output_file);

        // если не получилось вывести - выходим
        if (result != 0) return 1;
    } else {  // код родительского процесса
        char *parent_output_file = "parent.txt";  // пишем в файл parents.txt

        // пытаемся скопировать файл, закрываем файл из которого копировали
        int result = copy_file(input_file, parent_output_file);
        fclose(input_file);

        wait(NULL);  // ждем завершения дочернего процесса, если что-то не так -
                     // выходим
        if (result != 0) return 1;

        // пытаемся вывести файл
        result = print_file(parent_output_file);
        if (result != 0) return 1;
    }

    return 0;
}