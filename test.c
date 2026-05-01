#include <assert.h>
#include <stdio.h>
#include "dfa.h"

DFA build_dfa() {
    DFA dfa;
    dfa_init(&dfa, 0);

    dfa_add_transition(&dfa, 0, '-', 1);

    for (char d = '0'; d <= '9'; d++) {
        dfa_add_transition(&dfa, 0, d, 2);
        dfa_add_transition(&dfa, 1, d, 2);
        dfa_add_transition(&dfa, 2, d, 2);
        dfa_add_transition(&dfa, 3, d, 4);
        dfa_add_transition(&dfa, 4, d, 4);
        dfa_add_transition(&dfa, 5, d, 4);
    }

    dfa_add_transition(&dfa, 0, '.', 5);
    dfa_add_transition(&dfa, 1, '.', 5);
    dfa_add_transition(&dfa, 2, '.', 3);

    dfa_add_accept_state(&dfa, 2);
    dfa_add_accept_state(&dfa, 4);

    return dfa;
}

int main() {
    DFA dfa = build_dfa();
    DFAStatus st;

    assert(dfa_check(&dfa, "123", &st) == 1);
    assert(dfa_check(&dfa, "-123", &st) == 1);
    assert(dfa_check(&dfa, "12.34", &st) == 1);
    assert(dfa_check(&dfa, ".5", &st) == 1);
    assert(dfa_check(&dfa, "-.5", &st) == 1);

    assert(dfa_check(&dfa, ".", &st) == 0);
    assert(dfa_check(&dfa, "-", &st) == 0);
    assert(dfa_check(&dfa, "12.", &st) == 0);

    printf("All tests passed!\n");
    return 0;
}
