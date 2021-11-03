#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "io_handlers.h"
#include "command_handlers.h"

int main()
{
    char *input = NULL;
    int id, fork_id;

    while (strcmp((input = get_formatted_command()), "exit") != 0)
    {
        fork_id = fork();
        if (fork_id == 0)
        {
            id = execute_command(input);
            if (id == FAILURE)
            {
                fprintf(stderr, "Error encountered while trying to execute the command");
                exit(-1);
            }
            return 0;
        }
        else
            wait(NULL);

        free(input);
    }
    return 0;
}
