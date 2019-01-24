#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Решение производится исходя из того, что стандарт языка C99 и
// файлы на языке C ситнаксически верны
// Первое допущение обусловлено наличием однострочных комментариев, второе -
// как-то не хочется писать супер умную парсилку для такой задачи

void save_new_file(char filename[], char * contents, unsigned contents_size)
{
    FILE * out;
    char new_filename[64] = {'\0'};

    for (int i = 0; i < filename[i] != '\0'; ++i)
    {
        if (filename[i] == '.')
        {
            new_filename[i] = '.';
            new_filename[i + 1] = 'w';
            new_filename[i + 2] = 'c';

            break;
        }
        new_filename[i] = filename[i];
    }

    out = fopen(new_filename, "w");
    fwrite(contents, sizeof(char), contents_size, out);
    fclose(out);
}

char * remove_comments_from_file(char filename[], unsigned * file_contents_size)
{
    // Выделим 50кБ для нового файла без комментариев
    char * new_file = calloc(50 * 1024 * 1024, sizeof(char));
    char current_symbol = ' ';
    char prev_symbol = ' ';
    char before_prev_symbol = ' ';

    int string_started = 0;
    int ignore_next_symbol = 0;
    int possible_comment_started = 0;
    int multiline_started = 0;
    int multiline_just_ended = 0;
    int line_started = 0;
    int in_comment = 0;

    int k = 0;

    FILE * file = fopen(filename, "r");

    current_symbol = fgetc(file);
    while (EOF != current_symbol)
    {
        // Символ открытия/закрытия строки
        if (current_symbol == '\"' && prev_symbol != '\\')
        {
            string_started = !string_started;
        }

        //  Многострочный комментарий
        if (prev_symbol == '/' && current_symbol == '*' && !string_started && !in_comment)
        {
            in_comment = 1;
            multiline_started = 1;
        }
        if (prev_symbol == '*' && current_symbol == '/' && in_comment)
        {
            in_comment = 0;
            multiline_started = 0;
            multiline_just_ended = 1;
        }

        // Однострочный комментарий
        if (prev_symbol == '/' && current_symbol == '/' && !in_comment && !string_started && !multiline_just_ended)
        {
            in_comment = 1;
            line_started = 1;
        }
        if (prev_symbol != '\\' && current_symbol == '\n' && line_started)
        {
            in_comment = 0;
            line_started = 0;
        }

        if (!in_comment)
        {
            if (!string_started && prev_symbol == '/')
            {
                if (multiline_just_ended)
                {
                    multiline_just_ended = 0;
                }
                else
                {
                    new_file[k++] = prev_symbol;
                }
            }
            if (string_started || current_symbol != '/')
            {
                new_file[k++] = current_symbol;
            }
        }

        prev_symbol = current_symbol;
        current_symbol = fgetc(file);
    }
    fclose(file);

    *file_contents_size = k;
    return new_file;
}

int main(int argc, char const *argv[])
{
    int n;
    FILE * project = fopen("project.txt", "r");
    fscanf(project, "%d", &n);

    for (int i = 0; i < n; ++i)
    {
        char filename[33];
        char * file_contents;

        unsigned file_contents_size;

        fscanf(project, "%s", filename);

        file_contents = remove_comments_from_file(filename, &file_contents_size);
        save_new_file(filename, file_contents, file_contents_size);
        free(file_contents);
    }

    return 0;
}
