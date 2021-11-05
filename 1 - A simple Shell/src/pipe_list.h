#include <stdlib.h>

struct node
{
    char **command;
    struct node *next;
};

typedef struct node *list_node;

struct list
{
    size_t elements;
    struct node *head;
    struct node *guard_node;
};

typedef struct list *pipe_list;

pipe_list init_list();

void append_list(pipe_list list, char **command);

void free_list(pipe_list list);

void print_list(pipe_list list);