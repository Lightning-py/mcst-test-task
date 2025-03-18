#include "options.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// допустимые параметры для опции --elbrus
const char *elbrus_options[] = {"1c+", "2c+", "2c3", "4c", "8c", "16c"};

// освобождение памяти для параметров опции --elbrus
void free_elbrus_options(options *opt, int a) {
    for (int i = 0; i < a; ++i) {
        free(opt->elbrusvals[i]);
    }

    free(opt->elbrusvals);
}

// функция для проверки валидности параметра опции --elbrus
bool check_valid_elbrus_option(char *option) {
    bool flag = false;
    for (size_t i = 0; i < sizeof(elbrus_options) / sizeof(char *); ++i) {
        if (strcmp(option, elbrus_options[i]) == 0) {
            flag = true;
            break;
        }
    }

    return flag;
}

// функция для добавления опции --elbrus
int add_elbrus_option(options *opt, char *option) {
    // проверка на валидность параметра и выход в случае невалидности
    if (!check_valid_elbrus_option(option)) {
        printf("Incorrect parameter: %s\n", option);
        free_elbrus_options(opt, opt->elbrus_counter - 1);
        return 2;
    }

    // пытаемся добавить опцию
    opt->elbrus_counter++;

    if (!opt->elbrusvals) {
        opt->elbrusvals = calloc(1, sizeof(char *));

        // если не получается расиширить массив - выходим без очищения памяти,
        // потому еще что не выделили ничего
        if (!opt->elbrusvals) return 1;
    } else {
        char **temp =
            realloc(opt->elbrusvals, opt->elbrus_counter * sizeof(char *));

        // не получилось расширить массив - чистим память и выходим
        if (!temp) {
            free_elbrus_options(opt, opt->elbrus_counter - 1);
            return 1;
        }

        opt->elbrusvals = temp;
    }

    // пытаемся добавить опцию в массив опций
    int len = strlen(option);
    opt->elbrusvals[opt->elbrus_counter - 1] = malloc((len + 1) * sizeof(char));

    // если не получилось добавить место для опции в массив
    if (!opt->elbrusvals[opt->elbrus_counter - 1]) {  // чистим память и выходим
        free_elbrus_options(opt, opt->elbrus_counter - 1);
        return 1;
    }

    strcpy(opt->elbrusvals[opt->elbrus_counter - 1], option);
    return 0;
}

// функция для сохранения не опций
bool set_non_options(options *opt, int argc, char **argv, int optind) {
    // пытаемся выделить место для не опций
    opt->non_options = calloc(argc - optind, sizeof(char *));

    if (!opt->non_options) return 1;

    for (int i = optind; i < argc; ++i) {
        // пытаемся выделить место для конкретной опции
        opt->non_options[i - optind] =
            malloc((strlen(argv[i]) + 1) * sizeof(char));

        if (!opt->non_options[i - optind]) {  // если не получилось - выходим
            free_non_options(opt, i - optind);

            return 1;
        }

        // если получилось - копируем опцию
        strcpy(opt->non_options[i - optind], argv[i]);
    }

    opt->non_options_counter = argc - optind;

    return 0;
}

// функция для освобождения массива с не опциями
// счетчик count нужен в тех случаях, когда происходит ошибка в процессе добавки
// опции в общий список чтобы очищать не всю память
void free_non_options(options *opt, int count) {
    for (int i = 0; i < count; ++i) {
        free(opt->non_options[i]);
    }

    free(opt->non_options);
    opt->non_options_counter = 0;
}

// функция для парсинга переданных опций командой строки
int parse_options(options *res, int argc, char *argv[]) {
    int opt;
    int option_index = 0;

    struct option long_options[] = {
        {"elbrus", required_argument, 0, 'e'},  // задаем длинную опцию
        {0, 0, 0, 0}                            //
    };

    // обрабатываем обычные короткие опции вместе с длинной
    while ((opt = getopt_long(argc, argv, "mcst", long_options,
                              &option_index)) != -1) {
        switch (opt) {
            case 'm':
                res->m = 1;
                break;
            case 'c':
                res->c = 1;
                break;
            case 's':
                res->s = 1;
                break;
            case 't':
                res->t = 1;
                break;
            case 'e':
                // если опция --elbrus пытаемся добавить опцию в список опций
                // elbrus
                int result = add_elbrus_option(res, optarg);

                // если операция завершилась неудачно - выходим
                if (result != 0) return result;

                break;
            case '?':
                // если нашли неизвестную опцию - сообщаем о ней на выводе и
                // выходим
                printf("Options are incorrect\n");

                free_elbrus_options(res, res->elbrus_counter);
                return 2;

                break;
            default:
                break;
        }
    }

    // парсим не опции
    bool result_non_options = set_non_options(res, argc, argv, optind);

    // если что-то пошло не так во время парсинга не опций - освобождаем память
    // по уже выделенным опциям --elbrus и выходим
    if (result_non_options) {
        free_elbrus_options(res, res->elbrus_counter);
        return 1;
    }

    return 0;
}

// функция для вывода информации об опциях
void print_options(options *opts) {
    printf("Options: ");
    if (opts->m) printf("m");
    if (opts->c) printf("c");
    if (opts->s) printf("s");
    if (opts->t) printf("t");

    printf("\n");

    if (opts->elbrus_counter != 0) {
        printf("Elbrus options: ");

        for (size_t i = 0; i < opts->elbrus_counter; ++i) {
            printf("%s", opts->elbrusvals[i]);
            if (i != opts->elbrus_counter - 1) printf(", ");
        }

        printf("\n");
    }

    if (opts->non_options_counter != 0) {
        printf("Non options: ");

        for (size_t i = 0; i < opts->non_options_counter; ++i) {
            printf("%s", opts->non_options[i]);
            if (i != opts->non_options_counter - 1) printf(", ");
        }

        printf("\n");
    }
}
