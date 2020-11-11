#include <stddef.h>
#include "matrix.h"

void rot_left(mat4, size_t, size_t);
void rot_right(mat4, size_t, size_t);
void copy_row(uint8_t*, const mat4, size_t);

void sub_bytes(mat4 a, const mat4 b) {
    for (size_t i=0; i<16; i++)
        a[i] = b[i];
}

void rot_bytes(mat4 a) {
    rot_left(a, 1, 1);
    rot_left(a, 2, 2);
    rot_right(a, 3, 1);
}

void rot_left(mat4 a, size_t row_index, size_t shift) {
    uint8_t temp_row[4];
    copy_row(temp_row, a, row_index);
    for (size_t i=0; i<4; i++)
        I(a, row_index, i) = temp_row[(i + shift) % 4];
}

void rot_right(mat4 a, size_t row_index, size_t shift) {
    uint8_t temp_row[4];
    copy_row(temp_row, a, row_index);
    for (size_t i=0; i<4; i++)
        I(a, row_index, i) = temp_row[(i - shift + 4) % 4];
}

void copy_row(uint8_t* dest, const mat4 src, size_t row_index) {
    for (size_t i=0; i<4; i++)
        dest[i] = I(src, row_index, i);
}
