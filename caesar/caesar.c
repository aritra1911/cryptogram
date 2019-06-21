#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include "../shifts/shifts.h"

void caesar(int, FILE*, int (*)(int, int));

int main(int argc, char* argv[]) {
    char* offset = NULL, *filename = NULL;
    bool flag_from_file = false;
    int (*shift)(int, int) = shift_right;  // assume encryption by default
    FILE* input = stdin;

    // parse command line arguments
    int c;
    while (true) {
        static struct option long_options[] = {
            { "offset",  required_argument, 0, 'o' },
            { "decrypt", no_argument,       0, 'd' },
            { 0, 0, 0, 0 }
        };
        // getopt_long stores the option index here.
        int option_index = 0;

        c = getopt_long(argc, argv, "do:", long_options, &option_index);

        // Detect the end of the options.
        if (c == -1) break;

        switch (c) {
            case   0:
            case '?': // getopt_long already printed an error message.
                break;
            case 'o': offset = optarg;    break;
            case 'd': shift = shift_left; break;

            default: abort();
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

    if (flag_from_file && (input = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "caesar: can't open %s\n", filename);
        return 1;
    }

    caesar(atoi(offset), input, shift);

    return 0;
}

void caesar(int offset, FILE* input, int (*shift)(int, int)) {
    int c, en_c;

    while ((c = getc(input)) != EOF) {
        if (isalpha(c))
            en_c = shift(c, offset);
        else
            en_c = c;
        putchar(en_c);
    }
}
