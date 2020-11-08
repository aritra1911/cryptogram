#include <ctype.h>
#include "shifts.h"

int adjust(int, int);
int mod_int(int, int);

int shift_right(int ch, int offset) {
    int c = ch + offset;
    return adjust(c, ch);
}

int shift_left(int ch, int offset) {
    int c = ch - offset;
    return adjust(c, ch);
}

int adjust(int c, int ch) {
    c -= islower(ch) ? 'a' : 'A';
    c = mod_int(c, 26);
    c += islower(ch) ? 'a' : 'A';
    return c;
}

int mod_int(int x, int y) {
    return ((x % y) + y) % y;
}
