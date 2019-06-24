#include <stdio.h>
#include <stdbool.h>

void base64(void);

int main(void) {
    base64();
    return 0;
}

void base64(void) {
    int delta = 0, read = 0;
    unsigned char octet, sextet = 0;

    while (true) {
        if (delta == 0) {
            if ((octet = getchar()) == EOF) break;
            delta += 8;
        }

        if (!read && delta >= 6) {
            printf("%d\n", octet >> 2);
            delta -= 6;
        } else if (!read) {
            read = (6 - delta);
            sextet = (octet & ((1 << delta) - 1)) << read;
            delta = 0;
        } else {
            sextet |= octet >> (8 - read);
            printf("%d\n", sextet);
            break;
        }
    }
}
