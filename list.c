#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char code[8];
    char name[256];
    struct Node* next;
} Node;

Node* insert(Node* head, const char* code, const char* name) {
    Node* n = malloc(sizeof(Node));
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

Node* delete(Node* head, const char* code) {
    Node *cur = head, *prev = NULL;

    while (cur) {
        if (strcmp(cur->code, code) == 0) {
            if (prev) prev->next = cur->next;
            else head = cur->next;
            free(cur);
            return head;
        }
        prev = cur;
        cur = cur->next;
    }
    return head;
}

void free_list(Node* head) {
    while (head) {
        Node* tmp = head;
        head = head->next;
        free(tmp);
    }
}
