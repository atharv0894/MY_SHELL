#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT 1024

void shell_loop(char **env);
char **input_parser(char *input);



int my_strcmp(const char *s1, const char *s2);
int shell_builtins(char **args, char **env, char *initial_directory);
void free_tokens(char **tokens);
int command_cd(char **args ,char *initial_directory);
int command_pwd();
int command_echo(char **args,char** env);  
int command_env(char** env);  
int command_which(char **args,char **env);
 