// Single-byte XOR Cipher Breaker
// Many thanks to https://arpitbhayani.me/blogs/decipher-single-xor for the implementation idea.

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

float fitting_quotient(const char*, size_t);
char* single_byte_xor(const char*, size_t, char);

const float dist_english[] = {
    8.2389258,    1.5051398,    2.8065007,    4.2904556,
    12.813865,    2.2476217,    2.0327458,    6.1476691,
    6.1476691,    0.1543474,    0.7787989,    4.0604477,
    2.4271893,    6.8084376,    7.5731132,    1.9459884,
    0.0958366,    6.0397268,    6.3827211,    9.1357551,
    2.7822893,    0.9866131,    2.3807842,    0.1513210,
    1.9913847,    0.0746517
};

int main(void) {
    char* text = malloc(sizeof *text);
    size_t length=0;
    int c;

    while ((c = getchar()) != EOF) {
        text = realloc(text, (length + 1) * sizeof *text);
        text[length++] = c;
    }

    char possible_key=0;
    float lowest_quotient = fitting_quotient(text, length);
    char* case_text;
    float quotient;

    for (size_t i=1; i<256; i++) {
        case_text = single_byte_xor(text, length, i);
        quotient = fitting_quotient(case_text, length);

        if (quotient < lowest_quotient) {
            lowest_quotient = quotient;
            possible_key = i;
        }

        free(case_text);
    }

    free(text);

    printf("Possible key : %d", possible_key);
    if (isprint(possible_key)) printf(" ('%c')", possible_key);
    putchar('\n');

    return 0;
}

float fitting_quotient(const char* text, size_t length) {
    float dist_text[26] = { 0.0 };

    for (size_t i=0; i<length; i++)
        if (isalpha(text[i]) && islower(text[i]))
            dist_text[text[i] - 'a'] += 100.0 / (float)length;

    float sum = 0.0;

    for (size_t i=0; i<26; i++)
        sum += fabs(dist_english[i] - dist_text[i]);

    return sum / 26.0;
}

char* single_byte_xor(const char* text, size_t length, char byte) {
    char* xored_text = malloc(length * sizeof *xored_text);

    for (size_t i=0; i<length; i++)
        xored_text[i] = text[i] ^ byte;

    return xored_text;
}
