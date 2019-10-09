#include <stdio.h>
#include <stdbool.h>


#define BASE64_CHARACTER_SET \
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

#define mask(delta) (1 << delta) - 1


void base64_encode(void);
void base64_decode(void);
bool convert_base64_to_index(unsigned char*, char, char*);


int main(void) {
    base64_decode();
    return 0;
}


void base64_encode(void) {
    int delta = 0, read = 0;
    unsigned char octet, sextet;

    char* char_set = BASE64_CHARACTER_SET;

    while (true) {
        if (!delta) {
            octet = getchar();
            if ((signed char)octet == EOF) {
                if (read) {
                    putchar(char_set[sextet]);
                    for (int i = 0; i < (6 - read); i += 2)
                        putchar('=');
                }
                putchar('\n');
                break;
            }
            else if (octet == '\n') continue;
            delta += 8;
        }

        if (!read && delta >= 6) {
            if (delta > 6)
                putchar(char_set[octet >> (delta - 6)]);
            else
                putchar(char_set[octet & mask(delta)]);
            delta -= 6;
        } else if (!read) {
            read = delta;
            sextet = (octet & mask(delta)) << (6 - read);
            delta = 0;  // we're done with this octet.
        } else {
            read = 6 - read;  // prepare to read the rest of sextet
            sextet |= octet >> (delta - read);
            putchar(char_set[sextet]);
            delta -= read;
            read = 0;  // we're done reading that sextet
        }
    }
}


void base64_decode(void) {
    int delta = 0, read = 0;
    unsigned char octet, sextet;

    while (true) {
        if (!delta) {
            char c = getchar();

            if (c == EOF) {
                putchar('\n');
                break;
            }
            else if (!convert_base64_to_index(&sextet, c, BASE64_CHARACTER_SET))
                continue;
            delta += 6;
        } else if (!read) {
            read = delta;
            octet = sextet << (8 - delta);
            delta = 0;  // we're done with this sextet
        } else {
            read = 8 - read;  // prepare to read the rest of the octet
            octet |= sextet >> (delta - read);
            putchar(octet);
            delta -= read;
            read = 0;  // we're done reading that octet
        }
    }
}


bool convert_base64_to_index(unsigned char* sextet, char literal,
                             char* char_set) {
    for (int i = 0; i < 64; i++)
        if (char_set[i] == literal) {
            *sextet = i;
            return true;
        }
    return false;
}
