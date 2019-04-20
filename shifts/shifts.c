#include <ctype.h>
#include "shifts.h"

int adjust(int, int);
int mod26(int x);

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
    c = mod26(c);
    c += islower(ch) ? 'a' : 'A';
    return c;
}

int mod26(int x) {
    int y = x % 26;
    if (y >= 0) return y;
    return 26 + y;
}
