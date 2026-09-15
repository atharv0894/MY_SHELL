#include "myshell.h"

char *read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;

    if (getline(&line, &bufsize, stdin) == -1)
    {
        // Handle EOF (Ctrl+D)
        free(line);
        return NULL;
    }

    // Remove the trailing newline character that getline adds
    int len = my_strlen(line);
    if (len > 0 && line[len - 1] == '\n')
    {
        line[len - 1] = '\0';
    }

    return line;
}