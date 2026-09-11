/* shell_builtins.c */
#include "myshell.h"

int shell_builtins(char **args, char **env, char *initial_directory)
{
    /* exit */
    if (my_strcmp(args[0], "exit") == 0)
    {
        exit(EXIT_SUCCESS);
    }
    /* cd */
    else if (my_strcmp(args[0], "cd") == 0)
    {
        command_cd(args, initial_directory);
    }
    /* pwd */
    else if (my_strcmp(args[0], "pwd") == 0)
    {
        command_pwd();
    }
    /* echo */
    else if (my_strcmp(args[0], "echo") == 0)
    {
        command_echo(args, env);
    }
    /* env */
    else if (my_strcmp(args[0], "env") == 0)
    {
        command_env(env);
    }
    /* which */
    else if (my_strcmp(args[0], "which") == 0)
    {
        command_which(args, env);
    }
    else
    {
        return 0; /* not a builtin — let the caller handle external exec */
    }

    return 1;
}