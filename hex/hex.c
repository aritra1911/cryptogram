#include <stdio.h>
#include <stdbool.h>

#define SEPARATOR ' '

void encode(FILE*);
void decode(FILE*);
int to_hex(int);
int to_bin(int);

int main() {
    decode(stdin);
}

void encode(FILE* input) {
    int c;
    bool space = false;

    while ((c = getc(input)) != EOF) {
        if (space) putchar(SEPARATOR);
        else space = true;

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
    return 'A' + (bin - 10);
}

int to_bin(int hex) {
    if (hex >= 'A') return hex - 'A' + 10;
    return hex - '0';
}
