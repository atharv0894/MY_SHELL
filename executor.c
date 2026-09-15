#include "myshell.h"

/* simple bounded copy, since we don't have my_strncpy yet */
static void copy_string(char *dest, const char *src, size_t size)
{
    size_t i = 0;

    if (size == 0)
        return;
    while (src[i] && i < size - 1)
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int execute_pipeline(char ***cmds, int num_cmds, char **env)
{
    int i;
    int in_fd = 0; // Keeps track of the read end of the previous pipe
    int fd[2];
    pid_t pid;

    for (i = 0; i < num_cmds; i++)
    {
        // Create a new pipe for all but the last command
        if (i < num_cmds - 1)
        {
            if (pipe(fd) < 0)
            {
                perror("pipe");
                return 1;
            }
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return 1;
        }

        if (pid == 0) // Child Process
        {
            // If there's a previous pipe, read from it instead of standard input
            if (in_fd != 0)
            {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            
            // If there's a next command, write to the pipe instead of standard output
            if (i < num_cmds - 1)
            {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]); // Child doesn't read from current pipe
                close(fd[1]); 
            }

            // Execute the command using your existing child_process helper
            if (child_process(cmds[i], env) == -1)
                fprintf(stderr, "%s: command not found\n", cmds[i][0]);
            
            _exit(EXIT_FAILURE);
        }

        // Parent Process: Clean up file descriptors
        if (in_fd != 0)
            close(in_fd); // Close previous read end
            
        if (i < num_cmds - 1)
        {
            close(fd[1]);  // Parent doesn't write to the pipe
            in_fd = fd[0]; // Save the read end for the next command in the loop
        }
    }

    // Wait for all child processes to finish
    for (i = 0; i < num_cmds; i++)
    {
        wait(NULL);
    }
    
    return 1;
}

int executor(char **args, char **env)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        /* Child process: never return from here */
        if (child_process(args, env) == -1)
            fprintf(stderr, "%s: command not found\n", args[0]);
        _exit(EXIT_FAILURE);
    }

    /* Parent process */
    if (waitpid(pid, &status, 0) == -1)
    {
        perror("waitpid");
        return 1;
    }
    if (WIFSIGNALED(status))
        printf("Process terminated by signal: %d\n", WTERMSIG(status));

    return 1;
}

int child_process(char **args, char **env)
{
    char *path_string;
    char **path_list;
  
    char full_path[MAX_PATH_LEN];
    int num_paths = 0;
    int i;

    /* absolute path or ./prog: execute directly */
    if (strchr(args[0], '/') != NULL)
    {
        execve(args[0], args, env);
        return -1;
    }

    path_string = get_path(env);
    if (path_string == NULL)
        return -1;

    path_list = split_paths(path_string, &num_paths);
    if (path_list == NULL)
        return -1;

    for (i = 0; i < num_paths; i++)
    {
        int dir_len = my_strlen(path_list[i]);
        int cmd_len = my_strlen(args[0]);

        if (dir_len + 1 + cmd_len >= MAX_PATH_LEN)
            continue;

        copy_string(full_path, path_list[i], sizeof(full_path));
        full_path[dir_len] = '/';
        copy_string(full_path + dir_len + 1, args[0],
                    sizeof(full_path) - dir_len - 1);

        if (access(full_path, X_OK) == 0)
        {
            execve(full_path, args, env);
            perror("executor");
            break;
        }
    }

    for (i = 0; i < num_paths; i++)
        free(path_list[i]);
    free(path_list);

    return -1;
}

char *get_path(char **env)
{
    int i = 0;

    while (env[i])
    {
        /* prefix match, not exact match */
        if (my_strncmp(env[i], "PATH=", 5) == 0)
            return env[i] + 5; /* skip "PATH=" */
        i++;
    }
    return NULL;
}

char **split_paths(char *path_string, int *num_paths)
{
    char **result;
    int count = 0;
    int i = 0;
    int len;

    if (path_string == NULL)
    {
        *num_paths = 0;
        return NULL;
    }

    len = my_strlen(path_string);
    result = malloc(MAX_INPUT * sizeof(char *));
    if (!result)
    {
        perror("malloc");
        return NULL;
    }

    while (i <= len)
    {
        int start = i;
        int entry_len;
        char *entry;
        int j;

        while (path_string[i] && path_string[i] != ':')
            i++;

        entry_len = i - start;
        entry = malloc(entry_len + 1);
        if (!entry)
        {
            perror("malloc");
            while (count > 0)
                free(result[--count]);
            free(result);
            return NULL;
        }

        for (j = 0; j < entry_len; j++)
            entry[j] = path_string[start + j];
        entry[entry_len] = '\0';

        result[count++] = entry;

        if (path_string[i] == ':')
            i++;
        else
            break;
    }

    *num_paths = count;
    return result;
}