#include "pipe_list.h"
#include <stdlib.h>
#include <stdio.h>

static list_node new_node(char **command);

pipe_list init_list()
{
    pipe_list new_list = malloc(sizeof(pipe_list));
    new_list->elements = 0;

    new_list->guard_node = new_node(NULL);
    new_list->head = NULL;

    return new_list;
}

static list_node new_node(char **command)
{
    list_node new_node = malloc(sizeof(list_node));
    new_node->next = NULL;
    new_node->command = command;
    return new_node;
}

void append_list(pipe_list list, char **command)
{
    if (list->head == NULL)
    { /* First insertion */
        list->head = new_node(command);
        list->head->next = list->guard_node;
        list->guard_node->next = list->head;
    }
    else
    {
        list->guard_node->next->next = new_node(command);
        list->guard_node->next->next->next = list->guard_node;
        list->guard_node->next = list->guard_node->next->next;
    }
    list->elements++;
    return;
}

void free_list(pipe_list list)
{
    list_node curr, prev;
    if (!list)
        return;
    curr = list->head;
    while (curr != list->guard_node)
    {
        prev = curr;
        curr = curr->next;
        
        free(prev);
    }
    
    free(curr);
    free(list);
    return;
}

void print_list(pipe_list list)
{
    list_node curr;
    int i;
    if (!list)
        return;
    curr = list->head;
    while (curr != list->guard_node)
    {
        for (i = 0; curr->command[i] != NULL; i++)
            printf("%s ", curr->command[i]);
        printf("\n");
        curr = curr->next;
    }
}