#include <stdio.h>
#include "dfa.h"

int main() {
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

    char input[100];
    printf("Введите строку: ");
    scanf("%s", input);

    DFAStatus status;
    int result = dfa_check(&dfa, input, &status);

    if (status == DFA_INVALID_SYMBOL) {
        printf("Ошибка: недопустимый символ\n");
    } else if (status == DFA_NO_TRANSITION) {
        printf("Ошибка: нет перехода\n");
    } else {
        if (result)
            printf("Это число\n");
        else
            printf("Это НЕ число\n");
    }

    return 0;
}
