#include "../minishell.h"


bool is_valid_export(char *str)
{
	int i;

	i = 0;
	if(!*str)
		return (false);
	if(*str == '=')
		return (false);
	while(str[i])
	{
		if(str[i] == '=')
			return (true);
		i++;
	}
	return (false);
}

t_env *get_env(t_env *env, char *str)
{
	int i;
	char *to_find;

	i = 0;
	while(str[i] && str[i] != '=')
		i++;
	while(env)
	{
		if(!ft_strncmp(str, env->value, i) && env->value[i] == '=')
			return (env);
		env = env->next;
	}
	return (NULL);
}

int export(t_env **env, char *exp_arg)
{
	t_env *find;
	char *final;
	int i;

	i = 0;
	if(!exp_arg)
	{
		printf("NOTHING EXPORT GOES HERE\n");
		return (0);
	}
	if(!is_valid_export(exp_arg))
		return (NONE);
	find = get_env(*env, exp_arg);
	if(find)
	{
		final = ft_strdup(exp_arg, MANUAL);
		if(!final)
			return (MALLOC_ERROR);
		free(find->value);
		find->value = final;
	}
	else
	{
		find = ft_lstnew_env(exp_arg);
		ft_lstadd_back_env(env, find);
	}
	return (0);
}