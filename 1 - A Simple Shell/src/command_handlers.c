/*
Author: Georgios Manos
Academic ID: 4333
E-mail: csd4333@csd.uoc.gr
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <fcntl.h>

#include "command_handlers.h"
#define MAX_PIPES 1000

static void close_pipes(int pipes[][2], int N);
/**
 * Executes given cmd string properly :)
 */
int normal_execution(char **command)
{
    int i;
    int fd1 = -1, fd2 = -1, append = 0;
    for (i = 0; command[i] != NULL; i++) /* Check for any redirections here */
    {
        if (command[i][0] == '<')
        {
            command[i] = NULL;
            i++;
            if (fd1 != -1)
            {
                if (close(fd1) < 0)
                {
                    perror("Closing previous input file");
                    exit(-1);
                }
            }
            fd1 = open(command[i], O_RDONLY);
            if (fd1 < 0)
            {
                perror("Redirecting input");
                exit(-1);
            }
            dup2(fd1, STDIN_FILENO);
        }
        else if (command[i][0] == '>')
        {
            append = command[i][1] == '>'; /* It will be either that or '\0' */
            command[i] = NULL;
            i++;
            if (fd2 != -1)
            {
                if (close(fd1) < 0)
                {
                    perror("Closing previous input file");
                    exit(-1);
                }
            }
            if (append)
                fd2 = open(command[i], O_WRONLY | O_CREAT | O_APPEND, 00777);
            else
                fd2 = open(command[i], O_WRONLY | O_CREAT | O_TRUNC, 00777);
            if (fd2 < 0)
            {
                perror("Redirecting output");
                exit(-1);
            }
            dup2(fd2, STDOUT_FILENO);
        }
    }
    if (fd1 != -1)
        close(fd1);
    if (fd2 != -1)
        close(fd2);
    if (execvp(command[0], command) == -1) /* Execute! */
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
        if (pipe(pipefds[i]) < 0) /* Create pipe between all processes */
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
            if (normal_execution(curr_command->command) == FAILURE)
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
    while (wait(NULL) > 0) /* Wait for all child processes to finish */
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
