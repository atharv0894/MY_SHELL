#include "myshell.h"

int main(int argc, char *argv[], char **env)
{
    (void)argc;
    (void)argv;
    shell_loop(env);

    return 0;
}