#include <stdio.h>
#include <stdlib.h>

typedef struct _Node {
    int letter;
    struct _Node* parent;
    int child;
    struct _Node* left;
    struct _Node* right;
} Node;

Node* newNode(int letter, Node* parent, int child) {
    // Allocate memory for new node
    Node* node = (Node*)malloc(sizeof(Node));

    // Assign data to this node
    node->letter = letter;
    node->parent = parent;
    node->child = child;

    // Initialize left and right children as NULL
    node->left = NULL;
    node->right = NULL;
    return(node);
}

void demorse(Node* root, char* code) {
    Node* ptr = root;
    int ch = *code;
    while (*code != '\0') {
        if (*code == ' ') {
            if (ch != -1)
                putchar(ch);
            else {
                fprintf(stdout, "\n%s\n", "Morse not in tree");
                return;
            }
            ptr = root;
        }
        else if (*code == '/')
            ch = ' ';
        else if (*code == '.' || *code == '-') {
            if (*code == '.')
                ptr = ptr->left;
            else if (*code == '-')
                ptr = ptr->right;

            if (ptr != NULL)
                ch = ptr->letter;
            else ch = -1;
        } else ch = *code;
        code++;
    }
    if (ch != -1)
        putchar(ch);
    else
        fprintf(stdout, "\n%s\n", "Morse not in tree");
}

int main() {
    Node* root = newNode('\0', NULL, '\0');
    root->left = newNode('e', root, '.');
    root->right = newNode('t', root, '-');

    Node* level1_left = root->left;
    level1_left->left = newNode('i', level1_left, '.');
    level1_left->right = newNode('a', level1_left, '-');

    Node* level1_right = root->right;
    level1_right->left = newNode('n', level1_right, '.');
    level1_right->right = newNode('m', level1_right, '-');

    Node* level2_left1 = level1_left->left;
    level2_left1->left = newNode('s', level2_left1, '.');
    level2_left1->right = newNode('u', level2_left1, '-');

    Node* level2_left2 = level1_left->right;
    level2_left2->left = newNode('r', level2_left2, '.');
    level2_left2->right = newNode('w', level2_left2, '-');

    Node* level2_right1 = level1_right->left;
    level2_right1->left = newNode('d', level2_right1, '.');
    level2_right1->right = newNode('k', level2_right1, '-');

    Node* level2_right2 = level1_right->right;
    level2_right2->left = newNode('g', level2_right2, '.');
    level2_right2->right = newNode('o', level2_right2, '-');

    demorse(root, "- . .-");
    putchar('\n');
    demorse(root, "... ..- .-. -. .- -- .");
    putchar('\n');
    demorse(root, ".-- --- .-. -..");
    putchar('\n');
    demorse(root, "... --- ...");
    putchar('\n');
    demorse(root, ".-- --- .-. -.-");
    putchar('\n');
    demorse(root, "-- --- --- .-.");
    putchar('\n');
    demorse(root, ".. / .- -- / .-. --- --- -");
    putchar('\n');

    return 0;
}
