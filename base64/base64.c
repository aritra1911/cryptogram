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

    while (true) {
        if (delta == 0) {
            octet = getchar();
            if ((signed char)octet == EOF) {
                printf("EOF\n");
                break;
            } else if (octet == '\n') {
                printf("\n");
                continue;
            }
            delta += 8;
        }

        if (!read && delta >= 6) {
            if (delta > 6)
                printf("%d\n", octet >> (delta - 6));
            else {
                unsigned char mask = (1 << delta) - 1;
                printf("%d\n", octet & mask);
            }
            delta -= 6;
        } else if (!read) {
            read = delta;
	    unsigned char mask = (1 << delta) - 1;
            sextet = (octet & mask) << (6 - read);
            delta = 0;  // we're done with this octet.
        } else {
            read = 6 - read;  // prepare to read the rest of sextet
            sextet |= octet >> (delta - read);
            printf("%d\n", sextet);
            delta -= read;
            read = 0;  // we're done reading that sextet
        }
    }
}
