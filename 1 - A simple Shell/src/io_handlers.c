#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "io_handlers.h"

#define MAX_INFO_SIZE 150
#define MAX_PATH_SIZE 100
#define MAX_LINE_SIZE 1024
#define MAX_ARGS 100
#define PAUSED 0
#define NORMAL 1

static char buffer[MAX_LINE_SIZE + 1];
static size_t buff_index = 0;
static int input_status = NORMAL;

/**
 * Returns a null terminated, dynamically allocated string, formatted as <user>@cs345sh/<dir>$.
 */
static char *get_info(void);

/**
 * Initializes Buffer with getline.
*/
static void init_buffer();

char *get_formatted_command(void)
{
    char *line, c;
    size_t line_index = 0;

    if (input_status != PAUSED)
        init_buffer();

    line = (char *)malloc((strlen(&(buffer[buff_index])) + 1) * sizeof(char));
    while (1)
    {
        c = buffer[buff_index++];
        switch (c)
        {
        case ' ': /* Remove extra spaces. Only add 1 if its not in the beginning of the command. */
            if (line_index != 0)
                line[line_index++] = c;
            while ((c = buffer[buff_index]) == ' ')
                buff_index++;
            continue;
        case ';': /* Return to execute the already read command before moving on */
            input_status = PAUSED;
            if (line_index > 0 && line[line_index - 1] == ' ') /* Remove any spaces in the end of the command :P */
                line_index--;
            line[line_index] = '\0';
            return line;
        case EOF:
        case '\n':
        case '\0': /* Those cases end up the same: The command line is finished. */
            input_status = NORMAL;
            if (line_index > 0 && line[line_index - 1] == ' ') /* Remove any spaces in the end of the command :P */
                line_index--;
            line[line_index] = '\0';
            return line;
        default:
            break;
        }
        assert(buff_index <= MAX_LINE_SIZE);
        line[line_index++] = c;
        assert(line_index <= buff_index);
    }
    assert(0);
}

char **tokenize_command(char *cmd, const char *delim)
{
    char **command = (char **)malloc((MAX_ARGS + 1) * sizeof(char *)), *token;
    int index = 0;
    /* get the first token */
    token = strtok(cmd, delim);

    /* walk through other tokens */
    while (token != NULL)
    {
        command[index++] = token;
        if (index == MAX_ARGS)
            break;
        token = strtok(NULL, delim);
    }
    command[index] = NULL;
    return command;
}

static char *get_info(void)
{
    char *info = (char *)malloc(MAX_INFO_SIZE * sizeof(char)), *path, *login = getlogin();
    path = (char *)malloc(sizeof(char) * MAX_PATH_SIZE);
    getcwd(path, MAX_PATH_SIZE);
    if (path == NULL)
    {
        fprintf(stderr, "Path size limit exceeded.\n");
    }
    sprintf(info, "\x1b[32m%s@cs345sh\x1b[36m%s\x1b[0m$ ", login, path);
    free(path);
    return info;
}

static void init_buffer()
{
    char *vanilla_inp, *info;
    buff_index = 0; /* Reset Buffer size */
    info = get_info();
    vanilla_inp = readline(info);

    if (vanilla_inp == NULL)
    { /* This occurs when input is just EOF character. Follow the typical proceedure just to avoid mem errors :) */
        vanilla_inp = malloc(2 * sizeof(char));
        *vanilla_inp = EOF;
        *(vanilla_inp + sizeof(char)) = '\0';
    }
    else
        add_history(vanilla_inp);
    free(info);

    strncpy(buffer, vanilla_inp, MAX_LINE_SIZE);
    free(vanilla_inp);
    buffer[MAX_LINE_SIZE] = '\0'; /* Just making sure buffer is null terminated. */
}