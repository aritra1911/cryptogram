#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include "tree_utils.h"
#include "morse_audio.h"

#define DEFAULT_WPM 15

void demorse(Node*, FILE*);
void morse(Node*, FILE*, int (*)(int));
void put_morse(Node*, int, bool*, int (*)(int), char*);
void json_head(int, int);
void json_tail();
char* strcat_return(char*, char*);

int previous_character = '\0';
static int json_flag = 0;

int main(int argc, char* argv[]) {
    FILE* fp;
    if ((fp = fopen("tree.dat", "r")) == NULL &&
        (fp = fopen("morse/tree.dat", "r")) == NULL) {

        puts("Could not read file");
        return 0;
    }
    Node* root = NULL;
    deserialize(&root, fp);
    fclose(fp);

    int c; char *filename, *output_filename = "morse.wav";
    bool decrypt = false, flag_from_file = false, audio = false;
    int wpm = 0, fwpm = 0, sample_rate = 8000;
    float frequency = 1000, amplitude = 16383;
    FILE* input = stdin;

    while (true) {
        static struct option long_options[] = {
            // These options set a flag
            { "json",             no_argument, &json_flag,   1 },
            // These options don't set a flag
            { "audio",            no_argument,          0, 'a' },
            { "decrypt",          no_argument,          0, 'd' },
            { "speed",            required_argument,    0, 's' },
            { "farnsworth-speed", required_argument,    0, 'f' },
            { "output-filename",  required_argument,    0, 'o' },
            { "sample-rate",      required_argument,    0, 'N' },
            { "frequency",        required_argument,    0, 'F' },
            { "amplitude",        required_argument,    0, 'A' },
            { 0, 0, 0, 0 }
        };
        // getopt_long stores the option index here.
        int option_index = 0;

        c = getopt_long(
            argc, argv,
            "ads:f:o:F:N:A:",
            long_options, &option_index
        );

        // Detect the end of the options.
        if (c == -1) break;

        switch (c) {
            case   0:                             break;
            case 'a': audio = true;               break;
            case 'd': decrypt = true;             break;
            case 's': wpm = atoi(optarg);         break;
            case 'f': fwpm = atoi(optarg);        break;
            case 'o': output_filename = optarg;   break;
            case 'N': sample_rate = atoi(optarg); break;
            case 'F': frequency = atoi(optarg);   break;
            case 'A': amplitude = atoi(optarg);   break;
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
            abort();
        }
        input = fp;
    }

    if (!decrypt) {
        if (!audio)
            morse(root, input, putchar);
        else {
            if (!wpm && !fwpm)
                wpm = fwpm = DEFAULT_WPM;  // default
            else if (!fwpm) fwpm = wpm;
            else if (!wpm) wpm = DEFAULT_WPM;  // default
            else if (fwpm > wpm) {
                fprintf(stderr,
                    "morse: Farnsworth speed cannot be greater than speed.\n"
                );
                abort();
            }

            if (json_flag) {
                json_head(wpm, fwpm);
                morse(root, input, putchar);
                json_tail();
                return 0;
            }

            init_audio(wpm, fwpm, sample_rate, frequency, amplitude);
            morse(root, input, write_code);
            export_audio(output_filename);
        }
    } else demorse(root, input);

    if (!audio) putchar('\n');

    return 0;
}

void demorse(Node* root, FILE* input) {
    Node* ptr = root;
    int c, ch='\0', r_c = '\0';
    while ((c = getc(input)) != EOF) {
        if (r_c != '\0') {
            putchar(r_c);
            r_c = '\0';
        }

        if (isspace(c) || c == '/') {
            putchar(ch);
            ptr = root;
            ch = (c == '/') ? ' ' : '\0';
            if (c == '\n') r_c = c;
        } else if (c == '.' || c == '-') {
            if (ch == ' ') {
                putchar(ch);
                ch = '\0';
            }
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

void morse(Node* root, FILE* input, int (*write_morse)(int)) {
    int c, r_c = '\0'; bool flag, sp = false;
    while ((c = tolower(getc(input))) != EOF) {
        flag = false;
        if (r_c != '\0') {
            write_morse(r_c);
            r_c = '\0'; sp = false;
        } else if (sp == true && c != '\n') {
            if (c != ' ')
                write_morse(' ');
            sp = false;
        }

        if (c == ' ') {
            write_morse('/');
            flag = true;
        } else if (c == '\n')
            r_c = '\n';
        else 
            put_morse(root, c, &flag, write_morse, "");

        if (flag && r_c == '\0' && c != ' ') sp = true;
    }
}

void put_morse(Node* node, int ch, bool* flag,
                int (*write_morse)(int), char* code) {

    if (node == NULL) return;
    if (node->character == ch) {
        while (*code != '\0')
            write_morse(*(code++));
        *flag = true;
    } else {
        if (!*flag) put_morse(
            node->left, ch, flag, write_morse, strcat_return(code, ".")
        );

        if (!*flag) put_morse(
            node->right, ch, flag, write_morse, strcat_return(code, "-")
        );
    }
}

void json_head(int wpm, int fwpm) {
    fprintf(stdout, "{\"wpm\":%d,\"fwpm\":%d,\"code\":\"", wpm, fwpm);
}

void json_tail() {
    fprintf(stdout, "\"}");
}

char* strcat_return(char* str1, char* str2) {
    char* str = malloc(strlen(str1)*sizeof(str));
    strcpy(str, str1);
    strcat(str, str2);
    return str;
}
