#include <stdint.h>
#define I(mat,i,j) mat[i + 4*j]

typedef uint8_t mat4[16];

void sub_bytes(mat4, const mat4);
void rot_bytes(mat4);
