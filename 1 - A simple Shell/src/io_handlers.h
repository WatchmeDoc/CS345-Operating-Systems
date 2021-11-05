

enum
{
    NORMAL = 0,
    PIPE_STOP = 1,
    COLON_STOP = 2
} input_status;

/**
 * Returns a null terminated, dynamically allocated string given from stdin. MAX_SIZE == 1024 
 */
char *get_formatted_command(void);

char **tokenize_command(char *cmd, const char *delim);
