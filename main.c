#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_CITIES 1000      // максимальное количество городов
#define MAX_ROADS  10000     // максимальное количество дорог

// Структура для хранения дороги (ребра графа)
typedef struct {
    int from, to;
    int length;
} Road;

// Структура для хранения информации о текущем состоянии присоединения города
typedef struct {
    int owner;          // какое государство владеет городом (-1 = не занят)
    int distToOwner;    // минимальное расстояние до текущего множества своего государства (для неприсоединенных)
} CityState;

// Глобальные данные
int n, m;                       // количество городов и дорог
Road roads[MAX_ROADS];          // массив всех дорог
int roadCount[MAX_CITIES];      // сколько дорог выходит из города i
int adj[MAX_CITIES][MAX_CITIES]; // матрица смежности (храним длину дороги, 0 если нет дороги)

CityState cities[MAX_CITIES];   // состояние каждого города

int k;                          // количество государств (столиц)
int capitals[MAX_CITIES];       // номера столиц (индексы городов)

// Массивы для вывода результата
int* stateCities[MAX_CITIES];   // списки городов для каждого государства
int stateSize[MAX_CITIES];      // размер списка для каждого государства

// Инициализация графа и состояния
void init() {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            adj[i][j] = 0;
        }
        roadCount[i] = 0;
        cities[i].owner = -1;
        cities[i].distToOwner = INT_MAX;
    }
}

// Загрузка данных из файла (можно задать имя файла в коде или читать из stdin)
void loadData() {
    int i, from, to, len;

    // Читаем n и m
    scanf("%d %d", &n, &m);

    init();

    // Читаем дороги
    for (i = 0; i < m; i++) {
        scanf("%d %d %d", &from, &to, &len);
        from--; to--; // переводим в 0-индексацию
        roads[i].from = from;
        roads[i].to = to;
        roads[i].length = len;

        // Заполняем матрицу смежности
        adj[from][to] = len;
        adj[to][from] = len;

        // увеличиваем счетчики дорог (для списка смежности, если нужно)
        roadCount[from]++;
        roadCount[to]++;
    }

    // Читаем k и столицы
    scanf("%d", &k);
    for (i = 0; i < k; i++) {
        scanf("%d", &capitals[i]);
        capitals[i]--;
        cities[capitals[i]].owner = i;            // столица принадлежит государству i
        cities[capitals[i]].distToOwner = 0;      // расстояние до своего государства = 0
    }

    // Подготовка массивов для вывода
    for (i = 0; i < k; i++) {
        stateCities[i] = (int*)malloc(n * sizeof(int));
        stateSize[i] = 0;
        // сразу добавляем столицу
        stateCities[i][stateSize[i]++] = capitals[i] + 1; // назад в 1-индексацию для вывода
    }
}

// Обновление минимальных расстояний до ближайшего государства для всех свободных городов
void updateDistances() {
    int i, j;
    for (i = 0; i < n; i++) {
        if (cities[i].owner != -1) continue; // уже занят

        int minDist = INT_MAX;
        // проходим по всем соседям i
        for (j = 0; j < n; j++) {
            if (adj[i][j] != 0 && cities[j].owner != -1) { // сосед занят
                int distToState = cities[j].distToOwner + adj[i][j];
                if (distToState < minDist) {
                    minDist = distToState;
                }
            }
        }
        cities[i].distToOwner = minDist;
    }
}

// Поиск ближайшего незанятого города к любому государству
// Возвращает (город, государство)
void findClosest(int* bestCity, int* bestState) {
    int minDist = INT_MAX;
    *bestCity = -1;
    *bestState = -1;

    for (int i = 0; i < n; i++) {
        if (cities[i].owner != -1) continue; // только свободные
        if (cities[i].distToOwner < minDist) {
            minDist = cities[i].distToOwner;
            *bestCity = i;

            // нужно определить, к какому именно государству этот город ближе
            int minState = -1;
            int minStateDist = INT_MAX;
            for (int j = 0; j < n; j++) {
                if (adj[i][j] != 0 && cities[j].owner != -1) {
                    int d = cities[j].distToOwner + adj[i][j];
                    if (d == minDist && minState == -1) {
                        minState = cities[j].owner;
                        minStateDist = d;
                    }
                }
            }
            *bestState = minState;
        }
    }
}

// Основной процесс распределения
void distribute() {
    int remaining = n - k; // сколько городов еще не занято

    while (remaining > 0) {
        updateDistances();   // пересчитываем расстояния для всех свободных городов
        int city, state;
        findClosest(&city, &state);

        if (city == -1) {
            // на всякий случай, если что-то пошло не так
            break;
        }

        // Присоединяем city к state
        cities[city].owner = state;
        cities[city].distToOwner = 0; // теперь он принадлежит государству

        // Добавляем в результат
        stateCities[state][stateSize[state]++] = city + 1;
        remaining--;
    }
}

// Вывод результата в требуемом формате
void printResult() {
    for (int i = 0; i < k; i++) {
        printf("State %d:", i + 1);
        for (int j = 0; j < stateSize[i]; j++) {
            printf(" %d", stateCities[i][j]);
        }
        printf("\n");
        free(stateCities[i]); // освобождаем память
    }
}

int main() {
    printf("Enter data (n m, then roads, then k, then capitals):\n");
    loadData();
    distribute();
    printResult();
    return 0;
}
