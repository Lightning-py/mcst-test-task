#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "options.h"

int main(int argc, char *argv[]) {
    opterr = 0;
    options opts = {0};

    int result = parse_options(&opts, argc, argv);

    if (result != 0) {
        if (result == 1) printf("Not enought memory\n");
        exit(1);
    }

    print_options(&opts);

    free_elbrus_options(&opts, opts.elbrus_counter);
    free_non_options(&opts, opts.non_options_counter);

    return 0;
}