#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CODE_LEN 8
#define NAME_LEN 256

typedef struct Node {
    char code[CODE_LEN];
    char name[NAME_LEN];
    int height;
    struct Node* left;
    struct Node* right;
} Node;

// ---------------- AVL utilities ----------------

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(Node* n) {
    return n ? n->height : 0;
}

Node* newNode(const char* code, const char* name) {
    Node* n = (Node*)malloc(sizeof(Node));
    strcpy(n->code, code);
    strcpy(n->name, name);
    n->left = n->right = NULL;
    n->height = 1;
    return n;
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(Node* n) {
    return n ? height(n->left) - height(n->right) : 0;
}

// ---------------- Insert ----------------

Node* insert(Node* node, const char* code, const char* name) {
    if (!node)
        return newNode(code, name);

    int cmp = strcmp(code, node->code);

    if (cmp < 0)
        node->left = insert(node->left, code, name);
    else if (cmp > 0)
        node->right = insert(node->right, code, name);
    else {
        strcpy(node->name, name);
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && strcmp(code, node->left->code) < 0)
        return rightRotate(node);

    if (balance < -1 && strcmp(code, node->right->code) > 0)
        return leftRotate(node);

    if (balance > 1 && strcmp(code, node->left->code) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(code, node->right->code) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// ---------------- Find ----------------

Node* find(Node* root, const char* code) {
    if (!root) return NULL;

    int cmp = strcmp(code, root->code);

    if (cmp == 0) return root;
    if (cmp < 0) return find(root->left, code);
    return find(root->right, code);
}

// ---------------- Min value node ----------------

Node* minValue(Node* node) {
    Node* current = node;
    while (current->left)
        current = current->left;
    return current;
}

// ---------------- Delete ----------------

Node* deleteNode(Node* root, const char* code) {
    if (!root) return root;

    int cmp = strcmp(code, root->code);

    if (cmp < 0)
        root->left = deleteNode(root->left, code);
    else if (cmp > 0)
        root->right = deleteNode(root->right, code);
    else {
        if (!root->left || !root->right) {
            Node* temp = root->left ? root->left : root->right;

            if (!temp) {
                free(root);
                return NULL;
            } else {
                *root = *temp;
                free(temp);
            }
        } else {
            Node* temp = minValue(root->right);

            strcpy(root->code, temp->code);
            strcpy(root->name, temp->name);

            root->right = deleteNode(root->right, temp->code);
        }
    }

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// ---------------- Save (in-order) ----------------

void saveInOrder(Node* root, FILE* f) {
    if (!root) return;
    saveInOrder(root->left, f);
    fprintf(f, "%s:%s\n", root->code, root->name);
    saveInOrder(root->right, f);
}

// ---------------- Load file ----------------

Node* loadFile(const char* filename, int* count) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Не удалось открыть файл\n");
        exit(1);
    }

    Node* root = NULL;
    char line[512];
    *count = 0;

    while (fgets(line, sizeof(line), f)) {
        char* sep = strchr(line, ':');
        if (!sep) continue;

        *sep = '\0';

        char* code = line;
        char* name = sep + 1;

        name[strcspn(name, "\n")] = 0;

        root = insert(root, code, name);
        (*count)++;
    }

    fclose(f);
    return root;
}

// ---------------- CLI ----------------

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Использование: %s airports.txt\n", argv[0]);
        return 1;
    }

    int count = 0;
    Node* root = loadFile(argv[1], &count);

    printf("Загружено %d аэропортов. Система готова к работе.\n\n", count);

    char input[512];

    while (1) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) break;

        input[strcspn(input, "\n")] = 0;

        if (strncmp(input, "find ", 5) == 0) {
            char* code = input + 5;
            Node* res = find(root, code);

            if (res)
                printf("%s → %s\n", res->code, res->name);
            else
                printf("Аэропорт с кодом '%s' не найден в базе.\n", code);
        }

        else if (strncmp(input, "add ", 4) == 0) {
            char* data = input + 4;
            char* sep = strchr(data, ':');

            if (!sep) {
                printf("Неверный формат add\n");
                continue;
            }

            *sep = '\0';
            char* code = data;
            char* name = sep + 1;

            root = insert(root, code, name);
            printf("Аэропорт '%s' добавлен в базу.\n", code);
        }

        else if (strncmp(input, "delete ", 7) == 0) {
            char* code = input + 7;
            root = deleteNode(root, code);
            printf("Аэропорт '%s' удалён из базы.\n", code);
        }

        else if (strcmp(input, "save") == 0) {
            FILE* f = fopen(argv[1], "w");
            saveInOrder(root, f);
            fclose(f);
            printf("База сохранена.\n");
        }

        else if (strcmp(input, "quit") == 0) {
            break;
        }

        else {
            printf("Неизвестная команда\n");
        }
    }

    return 0;
}
