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

int getBalance(Node *n) {
    return n ? height(n->left) - height(n->right) : 0;
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

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left)
        current = current->left;
    return current;
}

Node* deleteNode(Node* root, char* code) {
    if (!root) return root;

    int cmp = strcmp(code, root->code);

    if (cmp < 0)
        root->left = deleteNode(root->left, code);
    else if (cmp > 0)
        root->right = deleteNode(root->right, code);
    else {
        if (!root->left || !root->right) {
            Node *temp = root->left ? root->left : root->right;

            if (!temp) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }

            free(temp);
        } else {
            Node* temp = minValueNode(root->right);
            strcpy(root->code, temp->code);
            free(root->name);
            root->name = strdup(temp->name);
            root->right = deleteNode(root->right, temp->code);
        }
    }

    if (!root) return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rotateRight(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return rotateLeft(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

Node* find(Node* root, char* code) {
    if (!root) return NULL;

    int cmp = strcmp(code, root->code);

    if (cmp == 0) return root;
    if (cmp < 0) return find(root->left, code);
    return find(root->right, code);
}

int countNodes(Node* root) {
    if (!root) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

void saveToFile(Node* root, FILE* f) {
    if (!root) return;

    saveToFile(root->left, f);
    fprintf(f, "%s:%s\n", root->code, root->name);
    saveToFile(root->right, f);
}

Node* loadFile(const char* filename, int *count) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Ошибка открытия файла\n");
        return NULL;
    }

    Node* root = NULL;
    char line[512];
    *count = 0;

    while (fgets(line, sizeof(line), f)) {
        char *code = strtok(line, ":");
        char *name = strtok(NULL, "\n");

        if (code && name) {
            root = insert(root, code, name);
            (*count)++;
        }
    }

    fclose(f);
    return root;
}

void cli(Node** root, const char* filename) {
    char command[512];

    while (1) {
        printf("> ");
        fgets(command, sizeof(command), stdin);

        if (strncmp(command, "find", 4) == 0) {
            char code[10];
            sscanf(command, "find %s", code);

            Node* n = find(*root, code);
            if (n)
                printf("%s → %s\n", code, n->name);
            else
                printf("Аэропорт с кодом '%s' не найден\n", code);
        }

        else if (strncmp(command, "add", 3) == 0) {
            char code[10], name[256];
            sscanf(command, "add %[^:]:%[^\n]", code, name);
            *root = insert(*root, code, name);
            printf("Аэропорт '%s' добавлен\n", code);
        }

        else if (strncmp(command, "delete", 6) == 0) {
            char code[10];
            sscanf(command, "delete %s", code);
            *root = deleteNode(*root, code);
            printf("Аэропорт '%s' удалён\n", code);
        }

        else if (strncmp(command, "save", 4) == 0) {
            FILE* f = fopen(filename, "w");
            saveToFile(*root, f);
            fclose(f);
            printf("База сохранена: %d аэропортов\n", countNodes(*root));
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

    int count = 0;
    Node* root = loadFile(argv[1], &count);

    printf("Загружено %d аэропортов. Система готова к работе.\n", count);

    cli(&root, argv[1]);

    return 0;
}
