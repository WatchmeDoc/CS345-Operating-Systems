#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include "io_handlers.h"
#include "command_handlers.h"

int main()
{
    char *input = NULL, **input_tokenized = NULL, delim[] = " ";
    int id, fork_id;
    pipe_list list;

    while (1)
    {

        input = get_formatted_command();
        
        input_tokenized = tokenize_command(input, delim);

        if (strcmp(input_tokenized[0], "exit") == 0)
            break;
        if (strcmp(input_tokenized[0], "cd") == 0)
        {
            if (chdir(input_tokenized[1]) == -1)
                printf("-bash: cd: %s: No such file or directory\n", input_tokenized[1]);
        }
        else
        {
            if (input_status == PIPE_STOP)
            { /* If we found any pipes (at all!) we create a list, save them and then create the multiple pipes execution. */
                list = init_list();
                while (input_status == PIPE_STOP)
                {
                    append_list(list, input_tokenized);
                    input = get_formatted_command();
                    input_tokenized = tokenize_command(input, delim);
                }
                append_list(list, input_tokenized);
                pipe_execution(list);
                free_list(list);
            }
            else
            { /* Otherwise, simply execute the given command :) */
                fork_id = fork();
                if (fork_id == -1)
                {
                    fprintf(stderr, "Could not create a child process!\n");
                    exit(-1);
                }
                if (fork_id == 0)
                {
                    id = normal_execution(input_tokenized);
                    if (id == FAILURE)
                    {
                        fprintf(stderr, "Error encountered while trying to execute the command\n");
                        _exit(-1);
                    }
                    assert(0);
                }
                else
                    wait(NULL);
            }
        }
        free(input);
        free(input_tokenized);
    }
    free(input);
    free(input_tokenized);
    return 0;
}
