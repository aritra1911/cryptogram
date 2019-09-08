#include <stdio.h>
#include <stdbool.h>

#define BASE64_CHARACTER_SET \
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

#define mask(delta) (1 << delta) - 1

void base64(void);

int main(void) {
    base64();
    return 0;
}

void base64(void) {
    int delta = 0, read = 0, i=0;
    unsigned char octet, sextet = 0;

    char* char_set = BASE64_CHARACTER_SET;

    while (true) {
        if (delta == 0) {
            octet = getchar();
            if ((signed char)octet == EOF) {
                if (read) {
                    putchar(char_set[sextet]);
                    for (int i = 0; i < (6 - read); i += 2)
                        putchar('=');
                }
                putchar('\n');
                break;
            }
            else if (octet == '\n') continue;
            delta += 8;
        }

        if (!read && delta >= 6) {
            if (delta > 6)
                putchar(char_set[octet >> (delta - 6)]);
            else
                putchar(char_set[octet & mask(delta)]);
            delta -= 6;
        } else if (!read) {
            read = delta;
            sextet = (octet & mask(delta)) << (6 - read);
            delta = 0;  // we're done with this octet.
        } else {
            read = 6 - read;  // prepare to read the rest of sextet
            sextet |= octet >> (delta - read);
            putchar(char_set[sextet]);
            delta -= read;
            read = 0;  // we're done reading that sextet
        }
    }
}
