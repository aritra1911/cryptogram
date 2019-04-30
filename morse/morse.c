#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "tree_utils.h"

void demorse(Node*, void*);
void put_morse(Node*, int, char*);
char* strcat_return(char*, char*);

int main() {
    FILE* fp;
    if ((fp = fopen("tree.txt", "r")) == NULL &&
        (fp = fopen("morse/tree.txt", "r")) == NULL) {

        puts("Could not read file");
        return 0;
    }
    Node* root = deserialize(fp);
    fclose(fp);

    demorse(root, stdin);
    putchar('\n');

    return 0;
}

void demorse(Node* root, void* input) {
    Node* ptr = root;
    int c, ch;
    while ((c = getc(input)) != EOF) {
        if (c == ' ') {
            if (ch != '\0')
                putchar(ch);
            else {
                fprintf(stdout, "\n%s\n", "Morse not in tree");
                return;
            }
            ptr = root;
        }
        else if (c == '/')
            ch = ' ';
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
    if (ch != '\0')
        putchar(ch);
    else
        fprintf(stdout, "\n%s\n", "Morse not in tree");
}

void put_morse(Node* node, int ch, char* code) {
    if (node == NULL) return;
    if (node->character == ch)
        printf("%s", code);
    else {
        put_morse(node->left, ch, strcat_return(code, "."));
        put_morse(node->right, ch, strcat_return(code, "-"));
    }
}

char* strcat_return(char* str1, char* str2) {
    char* str = malloc(strlen(str1)*sizeof(str));
    strcpy(str, str1);
    strcat(str, str2);
    return str;
}
