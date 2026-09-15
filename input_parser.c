#include "myshell.h"

char **split_pipes(char *input, int *num_cmds)
{
    size_t bufsize = 64;
    char **cmds = malloc(bufsize * sizeof(char *));
    int count = 0;
    
    // Split the input string using the pipe delimiter
    char *cmd = strtok(input, "|");
    while (cmd != NULL)
    {
        cmds[count++] = cmd;
        cmd = strtok(NULL, "|");
    }
    cmds[count] = NULL;
    *num_cmds = count;
    
    return cmds;
}

char **input_parser(char *input)
{
    size_t buffer_size = MAX_INPUT;
    char **tokens = malloc(buffer_size * sizeof(char *));
    char *token = NULL;
    size_t position = 0;
    size_t token_lenght = 0;
    size_t i = 0;

    if (!tokens)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while (input[i] != '\0' && position < buffer_size - 1)
    {
        // Skip leading spaces
        while (input[i] == ' ')
            i++;

        if (input[i] == '\0')
            break;

        // Measure the word (non-space run)
        token = &input[i];
        token_lenght = 0;
        while (input[i] && input[i] != ' ')
        {
            token_lenght++;
            i++;
        }

        tokens[position] = malloc((token_lenght + 1) * sizeof(char));
        if (!tokens[position])
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        for (size_t j = 0; j < token_lenght; j++)
            tokens[position][j] = token[j];
        tokens[position][token_lenght] = '\0';

        position++;
    }

    tokens[position] = NULL;
    return tokens;
}

void free_tokens(char **tokens)
{
    if (!tokens)
    {
       return;
    }

    for (size_t i = 0; tokens[i] != NULL; i++)
        free(tokens[i]);
    free(tokens);
}