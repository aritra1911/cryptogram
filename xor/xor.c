#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>

void xor(FILE*, char*);

int main(int argc, char* argv[]) {
    char *key = NULL, *filename = NULL;
    bool flag_from_file = false;
    FILE* input = stdin;

    // parse command line arguments
    int c;
    while (true) {
        static struct option long_options[] = {
            {"key", required_argument, 0, 'k' },
            { 0, 0, 0, 0 }
        };

        // getopt_long stores the option index here.
        int option_index = 0;

        c = getopt_long(argc, argv, "k:", long_options, &option_index);

        // Detect the end of the options.
        if (c == -1) break;

        switch (c) {
            case   0:
            case '?':  // getopt_long aleady printed an error message.
                break;
            case 'k': key = optarg; break;
            deafult: abort();
        }
    }

    if (key == NULL) {
        fprintf(stderr, "No key specified. Use -k to specify a key.\n");
        return 1;
    }

    if (argc > optind) {
        // input comes from a file
        filename = *(argv + optind);
        flag_from_file = true;
    }

    if (flag_from_file && (input = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "xor: can't open %s\n", filename);
        return 1;
    }

    xor(input, key);

    return 0;
}

void xor(FILE* input, char* key) {
    int c, i=0;
    while ((c = getc(input)) != EOF) {
        putchar(c ^ key[i++]);
        if (key[i] == '\0') i = 0;
    }
}
