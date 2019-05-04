#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include "tree_utils.h"
#include "morse_audio.h"

void demorse(Node*, FILE*);
void morse(Node*, FILE*, int (*)(int));
void put_morse(Node*, int, bool*, int (*)(int), char*);
char* strcat_return(char*, char*);

int main(int argc, char* argv[]) {
    FILE* fp;
    if ((fp = fopen("tree.txt", "r")) == NULL &&
        (fp = fopen("morse/tree.txt", "r")) == NULL) {

        puts("Could not read file");
        return 0;
    }
    Node* root = deserialize(fp);
    fclose(fp);

    char c; char *filename, *output_filename = "morse.wav";
    bool decrypt = false, flag_from_file = false, audio = false;
    int wpm = 0, fwpm = 0;
    FILE* input = stdin;

    while (true) {
        static struct option long_options[] = {
            { "audio",                      no_argument,       0, 'a' },
            { "decrypt",                    no_argument,       0, 'd' },
            { "words-per-minute",           required_argument, 0, 'w' },
            { "fansworth-words-per-minute", required_argument, 0, 'f' },
            { "output-filename",            required_argument, 0, 'o' },
            { 0, 0, 0, 0 }
        };
        // getopt_long stores the option index here.
        int option_index = 0;

        c = getopt_long(argc, argv, "adw:f:o:", long_options, &option_index);

        // Detect the end of the options.
        if (c == -1) break;

        switch (c) {
            case 0: break;
            case 'a': audio = true;             break;
            case 'd': decrypt = true;           break;
            case 'w': wpm = atoi(optarg);       break;
            case 'f': fwpm = atoi(optarg);      break;
            case 'o': output_filename = optarg; break;
            case '?':
                // getopt_long already printed an error message.
                break;

            default:
                abort();
        }
    }

    // Get any remaining command line arguments (not options).
    if (optind < argc) {
        // input comes from file
        filename = *(argv + optind);
        flag_from_file = true;
    }

    if (flag_from_file) {
        if ((fp = fopen(filename, "r")) == NULL) {
            fprintf(stderr, "morse: can't open %s\n", filename);
            return 1;
        }
        input = fp;
    }

    if (!decrypt) {
        if (!audio)
            morse(root, input, putchar);
        else {
            init_audio(wpm, fwpm);
            morse(root, input, write_code);
            export_audio(output_filename);
        }
    } else demorse(root, input);

    putchar('\n');

    return 0;
}

void demorse(Node* root, FILE* input) {
    Node* ptr = root;
    int c, ch, r_c = '\0';
    while ((c = getc(input)) != EOF) {
        if (r_c != '\0') {
            putchar(r_c);
            r_c = '\0';
        }

        if (isspace(c)) {
            putchar(ch);
            ptr = root;
            ch = '\0';
            if (c == '\n') r_c = c;
        }
        else if (c == '/') ch = ' ';

        else if (c == '.' || c == '-') {
            if (c == '.')
                ptr = ptr->left;
            else if (c == '-')
                ptr = ptr->right;

            if (ptr != NULL)
                ch = ptr->character;
            else ch = '\0';
        }
    }
    putchar(ch);
}

void morse(Node* root, FILE* input, int (*putmorse)(int)) {
    int c, r_c = '\0'; bool flag, sp = false;
    while ((c = tolower(getc(input))) != EOF) {
        flag = false;
        if (r_c != '\0') {
            putmorse(r_c);
            r_c = '\0'; sp = false;
        } else if (sp == true && c != '\n') {
            putmorse(' '); sp = false;
        }

        if (c == ' ') { putmorse('/'); flag = true; }
        else if (c == '\n') r_c = '\n';
        else put_morse(root, c, &flag, putmorse, "");

        if (flag && r_c == '\0') sp = true;
    }
}

void put_morse(Node* node, int ch, bool* flag,
                int (*putmorse)(int), char* code) {

    if (node == NULL) return;
    if (node->character == ch) {
        while (*code != '\0')
            putmorse(*(code++));
        // printf("%s", code);
        *flag = true;
    } else {
        if (!*flag) put_morse(
            node->left, ch, flag, putmorse, strcat_return(code, ".")
        );

        if (!*flag) put_morse(
            node->right, ch, flag, putmorse, strcat_return(code, "-")
        );
    }
}

char* strcat_return(char* str1, char* str2) {
    char* str = malloc(strlen(str1)*sizeof(str));
    strcpy(str, str1);
    strcat(str, str2);
    return str;
}
