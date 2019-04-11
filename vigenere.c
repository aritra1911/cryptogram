#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include "shifts.h"

void vigenere(char*, void*);

int main(int argc, char* argv[]) {
    char *key = NULL, *filename = NULL;
    bool flag_from_file = false;

    // parse command line arguments
    char c;
    while ((c = getopt (argc, argv, "k:")) != -1) {
        switch (c) {
            case 'k':
                key = optarg;
                break;
            case '?':
                printf("%s\n", optarg);
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
            printf("vigenere: can't open %s\n", filename);
            return 1;
        }
        vigenere(key, fp);
    } else
        vigenere(key, stdin);

    return 0;
}

void vigenere(char* key, void* input) {
    int c, en_c, offset;

    for (int i = 0; (c = getc(input)) != EOF;) {
        // here i keeps track of current character in key
        if (i == strlen(key)) i = 0;
        if (isalpha(c)) {
            // if other character encountered in key, find the next alphabet
            while (!isalpha(*(key + i)))
                if (++i == strlen(key)) i = 0;
            offset = *(key + i);
            offset -= islower(offset) ? 'a' : 'A';
            en_c = shift_right(c, offset);
            i++;  // increment only when encryption occurs
        } else
            en_c = c;  // if not alphabet, don't change
        putchar(en_c);
    }
}
