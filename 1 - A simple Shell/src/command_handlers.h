#include "pipe_list.h"
#define FAILURE 0
#define SUCCESS 1

/**
 * Executes given cmd string properly :)
 */
int normal_execution(char **cmd);

int pipe_execution(pipe_list commands);
