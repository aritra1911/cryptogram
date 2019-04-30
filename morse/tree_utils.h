typedef struct _Node {
    int character;
    struct _Node* left;
    struct _Node* right;
} Node;

Node* new_node(int);
void serialize(Node*, FILE*);
Node* deserialize(FILE*);
void place_in_tree(Node*, int, char*);
void print_traversal(Node*);
