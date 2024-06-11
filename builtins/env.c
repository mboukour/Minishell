#include "../minishell.h"



int env(t_env *env)
{
	ft_lstiter_env(env, false);
	return (0);
}
