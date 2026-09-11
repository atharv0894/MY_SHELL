#include "myshell.h"

/* my_strcmp: 0 if equal, <0 if s1 < s2, >0 if s1 > s2 */
int my_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

/* my_strlen: returns -1 on NULL input */
int my_strlen(const char *str)
{
    int len = 0;

    if (str == NULL)
        return -1;

    while (*str)
    {
        len++;
        str++;
    }
    return len;
}

/* my_strncmp: like strncmp, comparing at most n characters */
int my_strncmp(const char *str1, const char *str2, size_t n)
{
    size_t i = 0;

    if (str1 == NULL || str2 == NULL)
        return 1;

    while (i < n && str1[i] && str2[i])
    {
        if (str1[i] != str2[i])
            return (unsigned char)str1[i] - (unsigned char)str2[i];
        i++;
    }

    if (i == n)
        return 0;

    return (unsigned char)str1[i] - (unsigned char)str2[i];
}

/* my_getenv: find NAME=VALUE in env array, return pointer to VALUE */
char *my_getenv(const char *name, char **env)
{
    int name_len;

    if (name == NULL || env == NULL)
        return NULL;

    name_len = my_strlen(name);
    if (name_len < 0)
        return NULL;

    for (size_t i = 0; env[i]; i++)
    {
        if (my_strncmp(env[i], name, (size_t)name_len) == 0 &&
            env[i][name_len] == '=')
        {
            return &env[i][name_len + 1];
        }
    }
    return NULL;
}

int command_cd(char **args, char *initial_directory)
{
    (void)initial_directory;

    if (args[1] == NULL)
    {
        char *home = getenv("HOME");

        if (home != NULL)
        {
            if (chdir(home) != 0)
                perror("cd");
        }
        else
        {
            fprintf(stderr, "cd: HOME not set\n");
        }
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

extern char **environ;

int command_env(char **env)
{
    (void)env;
    char **e = environ;

    while (*e != NULL)
    {
        printf("%s\n", *e);
        e++;
    }
    return 1;
}

int command_which(char **args, char **env)
{
    (void)env;

    if (args[1] == NULL)
        return 1;

    if (my_strcmp(args[1], "cd") == 0 || my_strcmp(args[1], "exit") == 0 ||
        my_strcmp(args[1], "pwd") == 0 || my_strcmp(args[1], "echo") == 0 ||
        my_strcmp(args[1], "env") == 0 || my_strcmp(args[1], "which") == 0 ||
        my_strcmp(args[1], "export") == 0 || my_strcmp(args[1], "unset") == 0)
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

int command_export(char **args, char ***env)
{
    (void)env;

    if (args[1] == NULL)
    {
        return 1;
    }

    char *equal = strchr(args[1], '=');

    if (equal != NULL)
    {
        *equal = '\0';
        if (setenv(args[1], equal + 1, 1) != 0)
            perror("export");
        *equal = '=';
    }
    else
    {
        printf("export: usage: export NAME=VALUE\n");
    }

    return 1;
}

int command_unset(char **args, char ***env)
{
    (void)env;

    if (args[1] != NULL)
    {
        if (unsetenv(args[1]) != 0)
            perror("unset");
    }

    return 1;
}