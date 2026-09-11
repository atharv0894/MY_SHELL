#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024

void shell_loop(char **env);
char **input_parser(char *input);
void free_tokens(char **tokens);

/* custom string/env helpers */
int my_strcmp(const char *s1, const char *s2);
int my_strlen(const char *str);
int my_strncmp(const char *str1, const char *str2, size_t n);
char *my_getenv(const char *name, char **env);

int shell_builtins(char **args, char **env, char *initial_directory);
int executor(char **args, char **env);
int command_cd(char **args, char *initial_directory);
int command_pwd(void);
int command_echo(char **args, char **env);
int command_env(char **env);
int command_which(char **args, char **env);
int command_export(char **args, char ***env);
int command_unset(char **args, char ***env);


///executor
#define MAX_PATH_LEN 1024
int child_process(char **args, char **env);
char *get_path(char **env);
char **split_paths(char *path_string, int *num_paths);