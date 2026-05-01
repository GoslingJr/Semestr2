#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------- LIST NODE ----------------

typedef struct Node {
    char code[8];
    char name[256];
    struct Node* next;
} Node;

// ---------------- LIST FUNCTIONS ----------------

Node* insert(Node* head, const char* code, const char* name) {
    Node* n = (Node*)malloc(sizeof(Node));
    strcpy(n->code, code);
    strcpy(n->name, name);
    n->next = head;
    return n;
}

Node* find(Node* head, const char* code) {
    while (head) {
        if (strcmp(head->code, code) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

Node* deleteNode(Node* head, const char* code) {
    Node *cur = head, *prev = NULL;

    while (cur) {
        if (strcmp(cur->code, code) == 0) {
            if (prev)
                prev->next = cur->next;
            else
                head = cur->next;

            free(cur);
            return head;
        }
        prev = cur;
        cur = cur->next;
    }
    return head;
}

void freeList(Node* head) {
    while (head) {
        Node* tmp = head;
        head = head->next;
        free(tmp);
    }
}

// ---------------- MAIN CLI ----------------

int main() {
    Node* head = NULL;

    char input[512];

    printf("List version ready\n");

    while (1) {
        if (!fgets(input, sizeof(input), stdin))
            break;

        input[strcspn(input, "\n")] = 0;

        // FIND
        if (strncmp(input, "find ", 5) == 0) {
            char* code = input + 5;
            Node* res = find(head, code);

            if (res)
                printf("%s -> %s\n", res->code, res->name);
            else
                printf("NOT FOUND\n");
        }

        // ADD
        else if (strncmp(input, "add ", 4) == 0) {
            char* data = input + 4;
            char* sep = strchr(data, ':');

            if (!sep) {
                printf("bad format\n");
                continue;
            }

            *sep = '\0';
            char* code = data;
            char* name = sep + 1;

            head = insert(head, code, name);
        }

        // DELETE
        else if (strncmp(input, "delete ", 7) == 0) {
            char* code = input + 7;
            head = deleteNode(head, code);
        }

        // QUIT
        else if (strcmp(input, "quit") == 0) {
            break;
        }

        else {
            printf("unknown command\n");
        }
    }

    freeList(head);
    return 0;
}
