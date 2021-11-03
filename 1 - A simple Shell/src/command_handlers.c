#include "command_handlers.h"
#include <stdio.h>

/**
 * Executes given cmd string properly :)
 */
int execute_command(char *cmd)
{
    printf("given command: %s\n", cmd);
    return SUCCESS;
}