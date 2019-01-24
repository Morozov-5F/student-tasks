#include <stdio.h>
#include <stdlib.h>

#define MAX_N 24

char solution[MAX_N];

// Будем решать полным перебором, данные, вроде как, позволяют
int enumeration_loop(int x[MAX_N],     // Все числа
                     int total_numbers, // Общее их количество
                     int current_number, // Номер текущего числа
                     char op,  // Операция, которая производится
                     int prev_step, // Предыдущий шаг
                     int desired_result) // То, что хотим получить (S) )
{
    int current_step = prev_step;
    // Условие выхода их рекурсии -- мы прошлись по всем числам
    if (total_numbers == current_number)
    {
        // Успешным считается только тот проход, когда мы достигли нужного результата
        if (prev_step == desired_result)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    // Применяем операцию, которую нам сказали сверху
    if (op == '+')
    {
        current_step += x[current_number];
    }
    else
    {
        current_step -= x[current_number];
    }

    // Пробуем сначала сложение, потом вычитание. В глобальный массив записываем решение
    if (0 == enumeration_loop(x, total_numbers, current_number + 1, '+', current_step, desired_result))
    {
        solution[current_number] = '+';
        return 0;
    }
    else
    {
        if (0 == enumeration_loop(x, total_numbers, current_number + 1, '-', current_step, desired_result))
        {
            solution[current_number] = '-';
            return 0;
        }
    }
    return 1;
}

int main()
{
    int n, s;
    int x[MAX_N];

    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
    {
        scanf("%d", &x[i]);
        solution[i] = ' ';
    }
    scanf("%d", &s);

    if (0 != enumeration_loop(x, n, 0, '+', 0, s))
    {
        printf("no solution\n");
    }
    else
    {
        for (int i = 0; i < n; ++i)
        {
            if (i != n - 1)
            {
                printf("%d %c ", x[i], solution[i]);
            }
            else
            {
                printf("%d ", x[i]);
            }
        }
        printf("= %d\n", s);
    }
}