#include "../minishell.h"

void *globalizer_env(int mode, void *env)
{
    static void *head;

    if(mode == SET)
        head = env;
    else
        return (head);
    return (NULL);
}
