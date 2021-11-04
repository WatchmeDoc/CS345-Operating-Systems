#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "command_handlers.h"

/**
 * Executes given cmd string properly :)
 */
int execute_command(char **command)
{

    int status;
    status = execvp(command[0], command);

    if (status == -1)
        return FAILURE;

    return SUCCESS;
}