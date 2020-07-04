#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <getopt.h>

#define SEPARATOR 0

void encode(FILE*);
void decode(FILE*);
int to_hex(int);
int to_bin(int);

int main(int argc, char* argv[]) {
    char* filename = NULL;
    bool flag_from_file = false, flag_decode = false;
    FILE* input = stdin;

    // parse command line arguments
    int c;
    while (true) {
        static struct option long_options[] = {
            { "decrypt", no_argument, 0, 'd' },
            { 0, 0, 0, 0 }
        };
        // getopt_long stores the option index here.
        int option_index = 0;

        c = getopt_long(argc, argv, "d", long_options, &option_index);

        // Detect the end of the options.
        if (c == -1) break;

        switch (c) {
            case   0:
            case '?':  // getopt_long already printed an error message
                break;
            case 'd': flag_decode=true; break;

            default: abort();
        }
    }

    if (argc > optind) {
        // input comes from file
        filename = argv[optind];
        flag_from_file = true;
    }

    if (flag_from_file && (input = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "hex: can't open %s\n", filename);
        return 1;
    }

    if (!flag_decode) encode(input);
    else decode(input);

    return 0;
}

void encode(FILE* input) {
    int c;
    bool space = false;

    while ((c = getc(input)) != EOF) {
        if (space) {
            if (SEPARATOR)
                putchar(SEPARATOR);
        } else
            space = true;

        putchar(to_hex(c >> 4));
        putchar(to_hex(c & 15));
    }
}

void decode(FILE* input) {
    int c, bin;
    bool read = false;

    while (true) {
        c = getc(input);
        if (c == EOF) break;
        if (c == SEPARATOR) continue;

        if (!read) {
            bin = to_bin(c) << 4;
            read = true;
        } else {
            putchar(bin | to_bin(c));
            read = false;
        }
    }
}

int to_hex(int bin) {
    if (bin < 10) return '0' + bin;
    return 'a' + (bin - 10);
}

int to_bin(int hex) {
    if (tolower(hex) >= 'a') return hex - (islower(hex) ? 'a' : 'A') + 10;
    return hex - '0';
}
