#include <stdio.h>
#include "dfa.h"

int main() {
    DFA dfa;
    dfa_init(&dfa, 0); // стартовое состояние = 0

    // если встретили '-' → идём в состояние 1
    dfa_add_transition(&dfa, 0, '-', 1);

    // добавляем переходы для цифр
    for (char d = '0'; d <= '9'; d++) {
        dfa_add_transition(&dfa, 0, d, 2); // из старта → число
        dfa_add_transition(&dfa, 1, d, 2); // после '-' → число
        dfa_add_transition(&dfa, 2, d, 2); // продолжаем число
        dfa_add_transition(&dfa, 3, d, 4); // после точки → дробная часть
        dfa_add_transition(&dfa, 4, d, 4); // продолжаем дробную
        dfa_add_transition(&dfa, 5, d, 4); // .5 → дробная
    }

    // переходы по точке
    dfa_add_transition(&dfa, 0, '.', 5); // .5
    dfa_add_transition(&dfa, 1, '.', 5); // -.5
    dfa_add_transition(&dfa, 2, '.', 3); // 12.34

    // принимающие состояния:
    dfa_add_accept_state(&dfa, 2); // целое число
    dfa_add_accept_state(&dfa, 4); // дробное число

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
