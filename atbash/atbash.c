#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "../shifts/shifts.h"

void atbash(FILE*);

int main(int argc, char* argv[]) {
    char* filename = NULL;
    FILE* fp;

    // parse command line arguments
    if (argc > 1)
        for (int i=1; i<argc; i++) {
            filename = argv[i];
            if (fp = fopen(filename, "r"))
                atbash(fp);
            else {
                fprintf(stderr, "atbash: %s: No such file\n", filename);
                return 1;
            }
        }
    else
        atbash(stdin);

    return 0;
}

void atbash(FILE* input) {
    int c, ch;

    while ((c = getc(input)) != EOF) {
        if (isalpha(c)) {
            ch = c - (islower(c) ? 'a' : 'A');
            c = shift_left(c, 2*ch + 1);
        }

        putchar(c);
    }
}
