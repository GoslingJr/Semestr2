#include "dfa.h"
#include <string.h>

void dfa_init(DFA* dfa, int start_state) {
    dfa->transition_count = 0;
    dfa->accept_count = 0;
    dfa->start_state = start_state;
}

void dfa_add_transition(DFA* dfa, int from, char symbol, int to) {
    dfa->transitions[dfa->transition_count++] = (Transition){from, symbol, to};
}

void dfa_add_accept_state(DFA* dfa, int state) {
    dfa->accept_states[dfa->accept_count++] = state;
}

bool is_accept(DFA* dfa, int state) {
    for (int i = 0; i < dfa->accept_count; i++) {
        if (dfa->accept_states[i] == state)
            return true;
    }
    return false;
}

int find_transition(DFA* dfa, int state, char symbol) {
    for (int i = 0; i < dfa->transition_count; i++) {
        if (dfa->transitions[i].from == state &&
            dfa->transitions[i].symbol == symbol) {
            return dfa->transitions[i].to;
        }
    }
    return -1;
}

bool dfa_check(DFA* dfa, const char* str, DFAStatus* status) {
    int current = dfa->start_state;

    for (int i = 0; i < strlen(str); i++) {
        char c = str[i];

        if (!(c == '-' || c == '.' || (c >= '0' && c <= '9'))) {
            *status = DFA_INVALID_SYMBOL;
            return false;
        }

        int next = find_transition(dfa, current, c);

        if (next == -1) {
            *status = DFA_NO_TRANSITION;
            return false;
        }

        current = next;
    }

    *status = DFA_OK;
    return is_accept(dfa, current);
}
