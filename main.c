#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char code[5];
    char *name;
    int height;
    struct Node *left;
    struct Node *right;
} Node;

int height(Node *n) {
    return n ? n->height : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Node* createNode(char* code, char* name) {
    Node* n = malloc(sizeof(Node));
    strcpy(n->code, code);
    n->name = strdup(name);
    n->left = n->right = NULL;
    n->height = 1;
    return n;
}

Node* rotateRight(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node* rotateLeft(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(Node *n) {
    return n ? height(n->left) - height(n->right) : 0;
}

Node* insert(Node* node, char* code, char* name) {
    if (!node) return createNode(code, name);

    int cmp = strcmp(code, node->code);

    if (cmp < 0)
        node->left = insert(node->left, code, name);
    else if (cmp > 0)
        node->right = insert(node->right, code, name);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && strcmp(code, node->left->code) < 0)
        return rotateRight(node);

    if (balance < -1 && strcmp(code, node->right->code) > 0)
        return rotateLeft(node);

    if (balance > 1 && strcmp(code, node->left->code) > 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && strcmp(code, node->right->code) < 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

Node* find(Node* root, char* code) {
    if (!root) return NULL;

    int cmp = strcmp(code, root->code);

    if (cmp == 0) return root;
    if (cmp < 0) return find(root->left, code);
    return find(root->right, code);
}

void saveToFile(Node* root, FILE* f) {
    if (!root) return;

    saveToFile(root->left, f);
    fprintf(f, "%s:%s\n", root->code, root->name);
    saveToFile(root->right, f);
}

Node* loadFile(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Ошибка открытия файла\n");
        return NULL;
    }

    Node* root = NULL;
    char line[512];

    while (fgets(line, sizeof(line), f)) {
        char *code = strtok(line, ":");
        char *name = strtok(NULL, "\n");

        if (code && name) {
            root = insert(root, code, name);
        }
    }

    fclose(f);
    return root;
}

void cli(Node* root, const char* filename) {
    char command[512];

    while (1) {
        printf("> ");
        fgets(command, sizeof(command), stdin);

        if (strncmp(command, "find", 4) == 0) {
            char code[10];
            sscanf(command, "find %s", code);

            Node* n = find(root, code);
            if (n)
                printf("%s → %s\n", code, n->name);
            else
                printf("Не найдено\n");
        }

        else if (strncmp(command, "add", 3) == 0) {
            char code[10], name[256];
            sscanf(command, "add %[^:]:%[^\n]", code, name);
            root = insert(root, code, name);
            printf("Добавлено\n");
        }

        else if (strncmp(command, "save", 4) == 0) {
            FILE* f = fopen(filename, "w");
            saveToFile(root, f);
            fclose(f);
            printf("Сохранено\n");
        }

        else if (strncmp(command, "quit", 4) == 0) {
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    Node* root = loadFile(argv[1]);

    printf("Система готова\n");

    cli(root, argv[1]);

    return 0;
}
