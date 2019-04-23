#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "../shifts/shifts.h"

void caesar(int, void*, int (*)(int, int));

int main(int argc, char* argv[]) {
    char* offset = NULL, *filename = NULL;
    bool flag_from_file = false;
    int (*shift)(int, int) = shift_right;  // assume encryption by default

    // parse command line arguments
    char c;
    while ((c = getopt(argc, argv, "do:")) != -1) {
        switch (c) {
            case 'o':
                offset = optarg;
                break;

            case 'd':
                shift = shift_left;
                break;
        }
    }

    if (offset == NULL) {
        fprintf(stderr, "No offset specified. Use -o to specify an offset.\n");
        return 1;
    }

    if (argc > optind) {
        // input comes from file
        filename = *(argv + optind);
        flag_from_file = true;
    }

    if (flag_from_file) {
        FILE* fp;
        if ((fp = fopen(filename, "r")) == NULL) {
            fprintf(stderr, "caesar: can't open %s\n", filename);
            return 1;
        }
        caesar(atoi(offset), fp, shift);
    } else
        caesar(atoi(offset), stdin, shift);

    return 0;
}

void caesar(int offset, void* input, int (*shift)(int, int)) {
    int c, en_c;

    while ((c = getc(input)) != EOF) {
        if (isalpha(c))
            en_c = shift(c, offset);
        else
            en_c = c;
        putchar(en_c);
    }
}
