#include <stdio.h>
#include "matrix.h"

void print_mat(mat4);

int main(void) {
    mat4 a = { 1, 0, 0, 0,
               0, 1, 0, 0,
               0, 0, 1, 0,
               0, 0, 0, 1 };

    mat4 b = { 1, 2, 3, 4,
               5, 6, 7, 8,
               9, 0, 9, 8,
               7, 6, 5, 4 };

    print_mat(a);
    print_mat(b);
    sub_bytes(a, b);
    print_mat(a);
    rot_bytes(a);
    print_mat(a);
    return 0;
}

void print_mat(mat4 a) {
    for (size_t i=0; i<4; i++) {
        for (size_t j=0; j<4; j++)
            printf("%u ", I(a, i, j));
        putchar('\n');
    }
    putchar('\n');
}
