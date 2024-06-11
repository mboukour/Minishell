#include "../minishell.h"

void remove_node(t_env **env, t_env *node_to_remove) 
{

    if (node_to_remove == *env)
        *env = node_to_remove->next;

    if (node_to_remove->prev != NULL)
        node_to_remove->prev->next = node_to_remove->next;

    if (node_to_remove->next != NULL)
        node_to_remove->next->prev = node_to_remove->prev;


    free(node_to_remove->value);
    free(node_to_remove);
}


int unset_core(t_env **env, char *to_unset)
{
	t_env *unset_env;

	if(!to_unset)
		return (1);
    if(!ft_strcmp("?", to_unset))
    {
        ft_putstr_fd(2, "unset: `?': not a valid identifier\n");
        return (0);
    }
	unset_env = get_env(*env, to_unset);
	if(!unset_env)
		return (0);
	remove_node(env, unset_env);
	return (0);
}


int unset(t_env **env, t_treenode *unset_root)
{
    t_arg *arg;

    arg = unset_root->args;
    if(!arg->next)
        return (0);
    while(arg)
    {
        unset_core(env, arg->content);
        arg = arg->next;
    }
    return (0);
}