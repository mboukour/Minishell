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


int unset(t_env **env, char *to_unset)
{
	t_env *unset_env;

	if(!to_unset)
		return (0);
	unset_env = get_env(*env, to_unset);
	if(!unset_env)
		return (0);
	remove_node(env, unset_env);
	return (0);
}