#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

#include "command_handlers.h"
#define MAX_PIPES 1000

static void close_pipes(int pipes[][2], int N);
/**
 * Executes given cmd string properly :)
 */
int normal_execution(char **command)
{
    if (execvp(command[0], command) == -1)
        return FAILURE;
    assert(0);
}

int pipe_execution(pipe_list commands)
{
    int i, pipefds[MAX_PIPES][2], pid, command_count, pipe_count;
    list_node curr_command;
    if (!commands)
        return FAILURE;

    command_count = commands->elements;
    pipe_count = command_count - 1;
    for (i = 0; i < pipe_count; i++) /* For 3 piped commands we have 2 pipes, for 4 piped commands we have 3 pipes etc. */
    {
        if (pipe(pipefds[i]) < 0)
        {
            perror("Error creating pipe");
            exit(-1);
        }
    }
    curr_command = commands->head;
    i = 0;
    while (curr_command != commands->guard_node)
    {
        pid = fork();
        if (pid < 0)
        {
            perror("Error creating child process.");
            exit(-1);
        }
        else if (pid == 0)
        { /* Child process */
            if (i != 0)
            { /* Not in first command */
                if (dup2(pipefds[i - 1][0], STDIN_FILENO) < 0)
                {
                    perror("Error redirecting input");
                    exit(-1);
                }
            }
            if (i != command_count - 1)
            { /* Not in last command */
                if (dup2(pipefds[i][1], STDOUT_FILENO) < 0)
                {
                    perror("Error redirecting output");
                    exit(-1);
                }
            }
            close_pipes(pipefds, pipe_count);
            if (execvp(curr_command->command[0], curr_command->command) == -1)
            {
                perror("Error while executing command in multiple pipes.");
                exit(-1);
            }
            assert(0);
        }
        i++;
        curr_command = curr_command->next;
    }
    close_pipes(pipefds, pipe_count);
    while (wait(NULL) > 0)
        ;
    return SUCCESS;
}

static void close_pipes(int pipes[][2], int N)
{
    int i;

    for (i = 0; i < N; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
}