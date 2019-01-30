#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_N 24

int check_intersection(int * board, unsigned size, int x, int y)
{
    int intersects = 0;
    for (int i = 0; i < size; ++i)
    {
        int diag_x_left  = x - (y - i);
        int diag_x_right = x + (y - i);

        if (diag_x_left < size && board[diag_x_left + size * i] == 2)
        {
            intersects = 1;
        }
        if (diag_x_right >= 0 && board[diag_x_right + size * i] == 2)
        {
            intersects = 1;
            break;
        }
    }
    return intersects;
}

// "Ставит" фигуру на доску и помечает все поля под боем как "занятые"
void occupy_board(int * board, unsigned size, int x, int y)
{
    for (int i = 0; i < size; ++i)
    {
        int diag_x_left  = x - (y - i);
        int diag_x_right = x + (y - i);

        if (y == i)
        {
            continue;
        }

        if (0 <= diag_x_left && diag_x_left < size)
        {
            board[diag_x_left + i * size] = 1;
        }
        if (0 <= diag_x_right && diag_x_right < size)
        {
            board[diag_x_right + i * size] = 1;
        }
    }
    board[x + size * y] = 2;
}

int find_unoccupied_spot(int * board, unsigned size, int *x, int *y)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            if (board[i + size * j] == 0 && 0 == check_intersection(board, size, i, j))
            {
                *x = i;
                *y = j;

                return 1;
            }
        }
    }
    return 0;
}

void print_all_occupied_positions(int * board, unsigned size, FILE * out)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            if (board[i + size * j] == 2)
            {
                fprintf(out, "(%d, %d) ", i, j);
            }
        }
    }
    fprintf(out, "\n");
}

int place_figure_on_board(int *board, unsigned size, int initial_figures, int figures_left, FILE * out)
{
    int *new_board = NULL;
    int new_x, new_y;

    int board_was_occupied = 0;
    int solution_found = 0;

    // Если нам больше нечего выставлять -- хорошо, выходим
    if (figures_left == 0)
    {
        print_all_occupied_positions(board, size, out);
        return 0;
    }

    // Выделяем ресурсы под новую площадку
    new_board = (int *)calloc(size * size, sizeof(int));

    // Скопируем себе доску, чтобы не портить исходную
    memcpy(new_board, board, sizeof(int) * size * size);

    while (find_unoccupied_spot(new_board, size, &new_x, &new_y))
    {
        board_was_occupied = 1;
        occupy_board(new_board, size, new_x, new_y);

        if (0 == place_figure_on_board(new_board, size, initial_figures, figures_left - 1, out))
        {
            // Решение найдено, но делать мы ничего тут не будем
            solution_found = 1;
        }

        board[new_x + size * new_y] = 3;
        memcpy(new_board, board, sizeof(int) * size * size);
    }

    free(new_board);

    if (!board_was_occupied)
    {
        return 1;
    }

    if (!solution_found)
    {
        return 1;
    }

    return 0;
}


int main(int argc, char const *argv[])
{
    unsigned int n, l, k;
    int x, y;

    int * board = NULL;

    FILE * input = fopen("input.txt", "r");
    FILE * output = fopen("output.txt", "w");
    fscanf(input, "%u %u %u", &n, &l, &k);

    board = (int *)calloc(n * n, sizeof(int));

    for (int i = 0; i < k; ++i)
    {
        fscanf(input, "%d %d", &x, &y);
        occupy_board(board, n, x, y);
    }
    fclose(input);

    if (0 != place_figure_on_board(board, n, l, l, output))
    {
        fprintf(output, "no solutions\n");
    }
    fclose(output);

    free(board);

    return 0;
}
