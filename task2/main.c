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
    char prev_symbol = ' ';

    char string_start_char = '\0';
    int string_started = 0;
    int multiline_started = 0;
    int multiline_just_ended = 0;
    int line_started = 0;
    int in_comment = 0;

    int k = 0;

    char * new_file = (char *)calloc(50 * 1024 * 1024 + 1, sizeof(char));
    char * old_file = (char *)calloc(50 * 1024 * 1024 + 2, sizeof(char));

    FILE * f = fopen(filename, "r");
    long fsize = 0;

    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    rewind(f); // то же, что и fseek(f, 0, SEEK_SET)

    fread(old_file, fsize, 1, f);
    fclose(f);

    for (int i = 0; i < fsize; ++i)
    {
        char current_symbol = old_file[i];
        char next_symbol = old_file[i + 1];
        // Символ открытия/закрытия строки. Вообще говоря, внутри строки могут быть экранированные кавычки:
        // "tets \"number 1\""
        if (!in_comment && (current_symbol == '\"' || current_symbol == '\''))
        {
            // Ищем предыдущие слеши экранирования, чтобы понять, "настоящая" ли это кавычка
            int slashes = 0;
            for (int j = i - 1; j > 0 && old_file[j] == '\\'; --j)
            {
                slashes++;
            }

            if (!string_started)
            {
                string_start_char = current_symbol;
                string_started = 1;
            }
            else
            {
                if (string_started && string_start_char == current_symbol && slashes % 2 == 0)
                {
                    string_start_char = '\0';
                    string_started = 0;
                }
            }
        }

        //  Многострочный комментарий
        if (prev_symbol == '/' && current_symbol == '*' && !string_started && !multiline_started && !line_started)
        {
            in_comment = 1;
            multiline_started = 1;
        }
        else if (prev_symbol == '*' && current_symbol == '/' && !string_started && multiline_started && !line_started)
        {
            in_comment = 0;
            multiline_started = 0;
            multiline_just_ended = 1;
        }
        // Однострочный комментарий
        else if (prev_symbol == '/' && current_symbol == '/' && !string_started && !multiline_just_ended)
        {
            in_comment = 1;
            line_started = 1;
        }
        else if (prev_symbol != '\\' && current_symbol == '\n' && line_started)
        {
            in_comment = 0;
            line_started = 0;
        }

        in_comment = line_started || multiline_started;

        if (!in_comment && !multiline_just_ended)
        {
            if (!string_started)
            {
                if ((current_symbol != '/' || next_symbol != '*') &&
                    (current_symbol != '/' || next_symbol != '/'))
                {
                    new_file[k++] = current_symbol;
                }
            }
            else
            {
                new_file[k++] = current_symbol;
            }
        }

        if (multiline_just_ended)
        {
            multiline_just_ended = 0;
        }

        prev_symbol = current_symbol;
    }
    *file_contents_size = k;
    free(old_file);
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
