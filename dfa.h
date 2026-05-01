#ifndef DFA_H
#define DFA_H

#include <stdbool.h>

#define MAX_TRANSITIONS 100
#define MAX_STATES 50

typedef struct {
    int from;
    char symbol;
    int to;
} Transition;

typedef enum {
    DFA_OK,
    DFA_INVALID_SYMBOL,
    DFA_NO_TRANSITION
} DFAStatus;

typedef struct {
    Transition transitions[MAX_TRANSITIONS];
    int transition_count;

    int accept_states[MAX_STATES];
    int accept_count;

    int start_state;
} DFA;

void dfa_init(DFA* dfa, int start_state);
void dfa_add_transition(DFA* dfa, int from, char symbol, int to);
void dfa_add_accept_state(DFA* dfa, int state);

bool dfa_check(DFA* dfa, const char* str, DFAStatus* status);

#endif
