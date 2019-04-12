#include <ctype.h>
#include "shifts.h"

#define mod(x, y) (y + x) % y

int shift_right(int ch, int offset) {
    int c = ch + offset;
    c -= islower(ch) ? 'a' : 'A';
    c %= 26;
    c += islower(ch) ? 'a' : 'A';
    return c;
}

int shift_left(int ch, int offset) {
    int c = ch - offset;
    c -= islower(ch) ? 'a' : 'A';
    c = mod(c, 26);
    c += islower(ch) ? 'a' : 'A';
    return c;
}
