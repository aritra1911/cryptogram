#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <getopt.h>
#include "../shifts/shifts.h"

#define WILDCARD '?'

void vigenere(char*, void*, int (*)(int, int));

int main(int argc, char* argv[]) {
    char *key = NULL, *filename = NULL;
    bool flag_from_file = false;
    int (*shift)(int, int) = shift_right;  // assume encryption by default
    FILE* input = stdin;

    // parse command line arguments
    int c;
    while (true) {
        static struct option long_options[] = {
            { "key",     required_argument, 0, 'k' },
            { "decrypt", no_argument,       0, 'd' },
            { 0, 0, 0, 0 }
        };
        // getopt_long stores the option index here.
        int option_index = 0;

        c = getopt_long(argc, argv, "dk:", long_options, &option_index);

        // Detect the end of the options.
        if (c == -1) break;

        switch (c) {
            case   0:
            case '?': // getopt_long already printed an error message.
                break;
            case 'k': key = optarg;       break;
            case 'd': shift = shift_left; break;

            default: abort();
        }
    }

    if (key == NULL) {
        fprintf(stderr, "No key specified. Use -k to specify a key.\n");
        return 1;
    }

    if (argc > optind) {
        // input comes from file
        filename = *(argv + optind);
        flag_from_file = true;
    }

    if (flag_from_file && (input = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "vigenere: can't open %s\n", filename);
        return 1;
    }

    vigenere(key, input, shift);

    return 0;
}

void vigenere(char* key, void* input, int (*shift)(int, int)) {
    int c, en_c, offset;

    for (int i = 0; (c = getc(input)) != EOF;) {
        // here i keeps track of current character in key
        if (i == strlen(key)) i = 0;
        if (isalpha(c)) {
            // if other character encountered in key, find the next alphabet
            while (!isalpha(*(key + i)) && *(key + i) != WILDCARD)
                if (++i == strlen(key)) i = 0;
            if (*(key + i) == WILDCARD) offset = 0;
            else {
                offset = *(key + i);
                offset -= islower(offset) ? 'a' : 'A';
            }
            en_c = shift(c, offset);
            i++;  // increment only when encryption occurs
        } else
            en_c = c;  // if not alphabet, don't change
        putchar(en_c);
    }
}
