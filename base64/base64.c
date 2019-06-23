#include <stdio.h>

void base64(void);

int main(void) {
    base64();
    return 0;
}

void base64(void) {
    int delta = 0;
    char octet, sextet;
    
    octet = getchar();
    sextet = octet >> 2;
    printf("%d\n", sextet);
}
