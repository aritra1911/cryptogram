#include <stdio.h>
#include <stdbool.h>

void base64(void);

int main(void) {
    base64();
    return 0;
}

void base64(void) {
    int delta = 0, read = 0, i=0;
    unsigned char octet, sextet = 0;

    while (i<28) {
        if (delta == 0) {
            if ((octet = getchar()) == EOF) break;
            delta += 8;
        }

        if (!read && delta >= 6) {
            if (delta > 6)
                printf("%d\n", octet >> (delta - 6));
            else {
                unsigned char mask = (1 << delta) - 1;
                printf("%d\n", octet & mask);
            }
            i++;  // register a sextet
            delta -= 6;
        } else if (!read) {
            read = delta;
            sextet = (octet & ((1 << delta) - 1)) << (6 - read);
            delta = 0;  // we're done with this octet.
        } else {
            read = 6 - read;  // prepare to read the rest of sextet
            sextet |= octet >> (delta - read);
            printf("%d\n", sextet);
            i++;  // register a sextet
            delta -= read;
            read = 0;  // we're done reading that sextet
        }
    }
}
