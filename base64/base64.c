#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>


#define BASE64_CHARACTER_SET \
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

#define mask(delta) (1 << delta) - 1


void base64_encode(FILE*);
void base64_decode(FILE*);
bool convert_base64_to_index(unsigned char*, char, char*);


int main(int argc, char* argv[]) {
    char *filename = NULL;
    bool flag_from_file = false, flag_decode = false;
    FILE* input = stdin;

    // parse command line arguments
    int c;
    while (true) {
        static struct option long_options[] = {
            { "decrypt", no_argument, 0, 'd' },
            { 0, 0, 0, 0 }
        };
        // getopt_long stores the option index here.
        int option_index = 0;

        c = getopt_long(argc, argv, "d", long_options, &option_index);

        // Detect the end of the options.
        if (c == -1) break;

        switch (c) {
            case   0:
            case '?':  // getopt_long already printed an error message
                break;
            case 'd': flag_decode = true; break;

            default: abort();
        }
    }

    if (argc > optind) {
        // input comes from file
        filename = argv[optind];
        flag_from_file = true;
    }

    if (flag_from_file && (input = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "base64: can't open %s\n", filename);
        return 1;
    }

    if (!flag_decode) base64_encode(input);
    else base64_decode(input);

    return 0;
}


void base64_encode(FILE* input_buffer) {
    int delta = 0, read = 0;
    unsigned char octet, sextet;

    char* char_set = BASE64_CHARACTER_SET;

    while (true) {
        if (!delta) {
            int c = getc(input_buffer);
            
            if (c == EOF) {
                if (read) {
                    putchar(char_set[sextet]);
                    for (int i = 0; i < (6 - read); i += 2)
                        putchar('=');
                }
                break;
            }
            octet = c;
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


void base64_decode(FILE* input_buffer) {
    int delta = 0, read = 0;
    unsigned char octet, sextet;

    while (true) {
        if (!delta) {
            int c = getc(input_buffer);

            if (c == EOF) break;
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
