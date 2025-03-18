#ifndef OPTIONS_H
#define OPTIONS_H

#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    bool m;
    bool c;
    bool s;
    bool t;
    size_t elbrus_counter;
    char **elbrusvals;
    size_t non_options_counter;
    char **non_options;
    size_t invalid_options_counter;
    char **invalid_options;
} options;

void free_elbrus_options(options *opt, int a);
int add_elbrus_option(options *opt, char *option);
bool set_non_options(options *opt, int argc, char **argv, int optind);
void free_non_options(options *opt, int count);
int parse_options(options *opt, int argc, char *argv[]);
bool check_valid_elbrus_option(char *option);
void print_options(options *opts);

#endif  // OPTIONS_H
