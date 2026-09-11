
#include "myshell.h"
// my_strcmp: A simple string comparison function

int my_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}
int command_cd(char **args, char *initial_directory)
{
    (void)initial_directory;

    if (args[1] == NULL)
    {
        if (getenv("HOME") != NULL)
            chdir(getenv("HOME"));
    }
    else
    {
        if (chdir(args[1]) != 0)
            perror("cd");
    }
    return 1;
}

int command_pwd(void)
{
    char *cwd = getcwd(NULL, 0);

    if (cwd != NULL)
    {
        printf("%s\n", cwd);
        free(cwd);
    }
    else
    {
        perror("pwd");
    }
    return 1;
}

int command_echo(char **args, char **env)
{
    (void)env;
    int i = 1;

    while (args[i] != NULL)
    {
        printf("%s", args[i]);
        if (args[i + 1] != NULL)
            printf(" ");
        i++;
    }
    printf("\n");
    return 1;
}

int command_env(char **env)
{
    while (*env != NULL)
    {
        printf("%s\n", *env);
        env++;
    }
    return 1;
}

int command_which(char **args, char **env)
{
    (void)env;

    if (args[1] == NULL)
        return 1;

    if (strcmp(args[1], "cd") == 0 || strcmp(args[1], "exit") == 0 ||
        strcmp(args[1], "pwd") == 0 || strcmp(args[1], "echo") == 0 ||
        strcmp(args[1], "env") == 0 || strcmp(args[1], "which") == 0)
    {
        printf("%s: shell builtin\n", args[1]);
        return 1;
    }

    char *path_env = getenv("PATH");
    char *path_copy = path_env ? strdup(path_env) : NULL;
    char *dir = path_copy ? strtok(path_copy, ":") : NULL;
    char full_path[MAX_INPUT];
    int found = 0;

    while (dir != NULL)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, args[1]);
        if (access(full_path, X_OK) == 0)
        {
            printf("%s\n", full_path);
            found = 1;
            break;
        }
        dir = strtok(NULL, ":");
    }

    if (!found)
        printf("%s: not found\n", args[1]);

    free(path_copy);
    return 1;
}