#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "tree_utils.h"

void demorse(Node*, void*);
void morse(Node*, void*);
void put_morse(Node*, int, bool*, char*);
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

    char c; char* filename;
    bool decrypt = false, flag_from_file = false;
    void* input = stdin;

    while ((c = getopt(argc, argv, "d")) != -1) {
        switch (c) {
            case 'd':
                decrypt = true;
                break;
        }
    }

    if (argc > optind) {
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

    if (!decrypt) morse(root, input);
    else demorse(root, input);

    putchar('\n');

    return 0;
}

void demorse(Node* root, void* input) {
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

void morse(Node* root, void* input) {
    int c, r_c = '\0'; bool flag, sp = false;
    while ((c = tolower(getc(input))) != EOF) {
        flag = false;
        if (r_c != '\0') {
            putchar(r_c);
            r_c = '\0'; sp = false;
        } else if (sp == true && c != '\n') {
            putchar(' '); sp = false;
        }

        if (c == ' ') { putchar('/'); flag = true; }
        else if (c == '\n') r_c = '\n';
        else put_morse(root, c, &flag, "");

        if (flag && r_c == '\0') sp = true;
    }
}

void put_morse(Node* node, int ch, bool* flag, char* code) {
    if (node == NULL) return;
    if (node->character == ch) {
        printf("%s", code);
        *flag = true;
    } else {
        if (!*flag) put_morse(
            node->left, ch, flag, strcat_return(code, ".")
        );

        if (!*flag) put_morse(
            node->right, ch, flag, strcat_return(code, "-")
        );
    }
}

char* strcat_return(char* str1, char* str2) {
    char* str = malloc(strlen(str1)*sizeof(str));
    strcpy(str, str1);
    strcat(str, str2);
    return str;
}
