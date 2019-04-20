#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include "../shifts/shifts.h"

#define WILDCARD '?'

void vigenere(char*, void*, int (*)(int, int));

int main(int argc, char* argv[]) {
    char *key = NULL, *filename = NULL;
    bool flag_from_file = false;
    int (*shift)(int, int) = shift_right;  // assume encryption by default

    // parse command line arguments
    char c;
    while ((c = getopt(argc, argv, "dk:")) != -1) {
        switch (c) {
            case 'k':
                key = optarg;
                break;

            case 'd':
                shift = shift_left;
                break;
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

    if (flag_from_file) {
        FILE* fp;
        if ((fp = fopen(filename, "r")) == NULL) {
            fprintf(stderr, "vigenere: can't open %s\n", filename);
            return 1;
        }
        vigenere(key, fp, shift);
    } else
        vigenere(key, stdin, shift);

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
