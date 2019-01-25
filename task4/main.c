#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Условие задачи какое-то неадекватное. Буду решать так, как понял:
// В области простарнства R2, ограниченной прямыми x = 0, y = 0, x = 1000, y = 1000, заданы
// N прямых вида x = k_i, параллельных оси абсцисс и M прямых вида y = l_j параллельных оси
// ординат. Пусть есть точка A с кординатами (x_0, y_0). Необходимо написать программу, которая
// позволяет определить местоположение точки относительно линий сетки.
// Входные данные: файл input.txt, состоящих из трех строк:
// 1) N, M -- число прямых
// 2) k_1 k_2 ... k_N -- прямые вида x = k_i (вещественные)
// 3) l_1 l_2 ... l_M -- прямые вида y = l_j
// 4) x y -- координаты точки (вещественные)
// Выходные данные: стандартный поток
// 1) Если точка находится вне области: "out of bounds"
// 2) Если точка находится на линиях сетки: "on the grid"
// 3) В противном случае, вывести индекс квадрата, в котором находится точка.
//    Квадраты нумеруются от начала координат. К примеру, если есть две прямые,
//    то квараты обозначаются следующим образом:
// 1000|---------|---------|
//     |         |         |
//     |   0,1   |   1,1   |
//  500|---------|---------|
//     |         |         |
//     |   0,0   |   1,0   |
//     |---------|---------|
//     0        500       1000


// В этой реализации бинарный поиск при отсутствии элемента в массиве вернет
// первое число, которое не больше искомого, это сделано для облегчения поиска
// квадратов
int binary_search(double * array, int start_idx, int end_idx, double elem)
{
    int middle_idx = (start_idx + end_idx) / 2;
    double middle;

    if (start_idx > end_idx)
    {
        return middle_idx;
    }

    middle = array[middle_idx];

    if (middle > elem)
    {
        return binary_search(array, start_idx, middle_idx - 1, elem);
    }
    if (middle < elem)
    {
        return binary_search(array, middle_idx + 1, end_idx, elem);
    }

    return middle_idx;
}

int check_is_on_grid(double *array, int size, double elem, int idx)
{
    // Проверяем, что точка не находится на внешних границах. Внешние границы
    // у нас в массиве на первом и последнем индексе. Еще проверим, что мы не попали на
    // заданную сетку -- числа с плавающей точкой правильнее сравнивать через модуль из-за
    // ошибок округления: 0.1 на самом деле 0.100000001 итд. 1e-14 -- это 10^-14
    if (idx == 0 || idx == size - 1 || fabs(array[idx] - elem) < 1e-14)
    {
        return 0;
    }
    return 1;
}

int main()
{
    FILE * input = fopen("input.txt", "r");

    unsigned N, M;

    double point_x, point_y;
    double * x = NULL;
    double * y = NULL;

    fscanf(input, "%u %u", &N, &M);

    x = calloc(N + 2, sizeof(double));
    y = calloc(M + 2, sizeof(double));

    // для удобства поиска, добавим нулевые и тысячные границы в массивы линий
    // Так мы найдем сразу же нулевые и последние квадраты
    x[0] = y[0] = 0;
    x[N - 1] = y[M - 1] = 1000;
    for (int i = 0; i < N; ++i)
    {
        fscanf(input, "%lf", &x[i + 1]);
    }
    for (int i = 0; i < M; ++i)
    {
        fscanf(input, "%lf", &y[i + 1]);
    }

    fscanf(input, "%lf %lf", &point_x, &point_y);
    fclose(input);

    if (point_x > 1000 || point_y > 1000 || point_x < 0 || point_y < 0)
    {
        printf("out of bounds\n");
        return 0;
    }

    int is_on_grid_x = 0, is_on_grid_y = 0;
    int point_idx_x = binary_search(x, 0, N - 1, point_x);
    int point_idx_y = binary_search(y, 0, M - 1, point_y);

    is_on_grid_x = check_is_on_grid(x, N, point_x, point_idx_x);
    is_on_grid_y = check_is_on_grid(y, M, point_y, point_idx_y);

    // Вывод для отладки, просто печатает, где была найдена точка

    // printf("X (%f): idx: %d, number: %f, on_grid: %s\n", point_x, point_idx_x,
    //        point_idx_x >= 0 && point_idx_x < N ? x[point_idx_x] : (double)point_idx_x,
    //        is_on_grid_x ? "TRUE" : "FALSE");
    // printf("Y (%f): idx: %d, number: %f, on_grid: %s\n", point_y, point_idx_y,
    //        point_idx_y >= 0 && point_idx_y < M ? y[point_idx_y] : (double)point_idx_y,
    //        is_on_grid_y ? "TRUE" : "FALSE");

    // Если требуется, чтобы выводилось "on_grid" когда точка в узлах, поменять "или" на "и"
    if (is_on_grid_x || is_on_grid_y)
    {
        printf("on the grid\n");
        return 0;
    }

    printf("Square: (%d, %d)\n", point_idx_x, point_idx_y);

    free(x);
    free(y);
    return 0;
}
