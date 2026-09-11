#include "myshell.h"

void shell_loop(char **env)
{
    (void)env;
    char *input = NULL;
    size_t input_size = 0;
    ssize_t bytes_read;
    char **args;
    char *initial_directory = getcwd(NULL, 0);

    while (1)
    {
        printf("[myshell]>");
        bytes_read = getline(&input, &input_size, stdin);

        // Handle EOF (Ctrl+D) or read error
        if (bytes_read == -1)
        {
            if (!feof(stdin))
                perror("Error reading input");
            break;
        }

        // Strip trailing newline left by getline
        if (bytes_read > 0 && input[bytes_read - 1] == '\n')
            input[bytes_read - 1] = '\0';

        args = input_parser(input);

        if (args[0] != NULL)
        {
            shell_builtins(args, env, initial_directory);
        }

        free_tokens(args);
    }

    // Free allocated buffers on exit
    free(input);
    free(initial_directory);
}