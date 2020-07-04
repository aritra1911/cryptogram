/* Given a file having several hex encoded binaries (each line is one single binary), output the line # of the one that
 * most likely was encrypted with a Single-Byte XOR cipher.
 *
 * Dependencies:
 * - Make sure to `make` hex & sbx_breaker before executing.
 */

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>

float get_fitting_quotient(char*, size_t);

int main(int argc, char** argv) {
    FILE* input=stdin;
    size_t i=0, l=0, line=0, lowest_quotient_line=0;
    char* hex = malloc(sizeof *hex);
    float quotient, lowest_quotient=-1.0;
    int c;

    if (argc > 1 && (input = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "sbx_detect: can't open %s\n", argv[1]);
        abort();
    }

    while ((c = getc(input)) != EOF) {
        if (c == '\n') {
            line++;
            hex[i++] = 0;

            quotient = get_fitting_quotient(hex, i);
            if (lowest_quotient == -1.0 || quotient < lowest_quotient) {
                lowest_quotient = quotient;
                lowest_quotient_line = line;
            }

            if (i > l) l = i;
            i = 0;

        } else {
            if (i+1 > l)
                hex = realloc(hex, (i+2) * sizeof *hex);
            hex[i++] = c;
        }
    }

    free(hex);

    printf("%ld", lowest_quotient_line);

    return 0;
}

float get_fitting_quotient(char* hex, size_t length) {
    char command[length + 54 + 1];  // I'm too lazy to use strlen()
    float quotient;

    sprintf(command, "echo %s | ./hex.out -d | ./sbx_breaker.out --get-quotient", hex);

    FILE* pipein;
    if ((pipein = popen(command, "r")) == NULL) {
        fprintf(stderr, "Failed to open pipe\n");
        abort();
    }

    fscanf(pipein, "%f", &quotient);

    int sig;
    if ((sig = pclose(pipein))) {
        fprintf(stderr, "Pipe returned %d\n", sig);
        abort();
    }

    return quotient;
}
