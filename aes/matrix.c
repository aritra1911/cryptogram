#include <stddef.h>
#include "matrix.h"

void sub_bytes(mat4 a, const mat4 b) {
    for (size_t i=0; i<16; i++)
        a[i] = b[i];
}
