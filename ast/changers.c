#include "../minishell.h"

int count_arg(t_arg *arg)
{
    int i;

    i = 0;
    while(arg)
    {
        i++;
        arg = arg->next;
    }
    return (i);
}

char **args_to_arr(t_arg *arg)
{
    int i;
    int size;
    char **result;

    i = 0;
    size = count_arg(arg);
    result = smart_malloc((size + 1) * sizeof(char *));
    while(arg)
    {
        result[i] = ft_strdup(arg->content, GC);
        i++;
        arg = arg->next;
    }
    result[i] = NULL;
    return (result);
}

