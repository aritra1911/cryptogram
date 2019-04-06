#include <stdio.h>
#include <string.h>
#include <ctype.h>

void vignere_pipe(char* key) {
    int c, en_c, offset;

    c = getchar();
    for (int i=0; c != EOF; c = getchar()) {
        if (i == strlen(key)) i = 0;
        if (isalpha(c)) {
            // if other character encountered in key, find the next alphabet
            while (!isalpha(*(key + i)))
                if (++i == strlen(key)) i = 0;

            offset = *(key + i);
            offset -= islower(offset) ? 'a' : 'A';
            en_c = c + offset;
            en_c -= islower(c) ? 'a' : 'A';
            en_c %= 26;
            en_c += islower(c) ? 'a' : 'A';
            i++;  // increment only when encryption occurs
        } else
            en_c = c;  // if not alphabet, don't change
        putchar(en_c);
    }
}

int main() {
    char* key = "SECRET KEY";  // TODO, use command line args

    vignere_pipe(key);
    return 0;
}
