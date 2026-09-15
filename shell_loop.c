#include "myshell.h"

void shell_loop(char **env)
{
    char *input;
    char *initial_directory = getcwd(NULL, 0);

    while (1)
    {
        printf("[myshell]>");
        fflush(stdout);

        input = read_line();

        if (input == NULL)   /* Ctrl+D */
        {
            printf("\n");
            break;
        }

        int num_cmds = 0;
        // Split by pipes first
        char **raw_cmds = split_pipes(input, &num_cmds);

        if (num_cmds == 1)
        {
            // No pipes: Execute normally (allows builtins to work properly)
            char **args = input_parser(raw_cmds[0]);
            if (args[0] != NULL)
                shell_builtins(args, env, initial_directory);
            free_tokens(args);
        }
        else if (num_cmds > 1)
        {
            // Pipes detected: Set up command array
            char ***pipeline_args = malloc(num_cmds * sizeof(char **));
            for (int i = 0; i < num_cmds; i++)
            {
                pipeline_args[i] = input_parser(raw_cmds[i]);
            }
            
            // Execute the pipeline
            execute_pipeline(pipeline_args, num_cmds, env);

            // Clean up memory
            for (int i = 0; i < num_cmds; i++)
                free_tokens(pipeline_args[i]);
            free(pipeline_args);
        }

        free(raw_cmds);
        free(input);
    }

    free(initial_directory);
}