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
    /* export */
    else if (my_strcmp(args[0], "export") == 0)
    {
        command_export(args, &env);
    }
    /* unset */
    else if (my_strcmp(args[0], "unset") == 0)
    {
        command_unset(args, &env);
     }
    
    else
    {
        //not builtin command,execute as exterenal command 
        executor(args, env);
    }

    return 0;
}