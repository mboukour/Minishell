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

void export_malloc_failure(t_env *env)
{
	ft_lstclear_env(env);
	smart_free();
	ft_putstr_fd(2, FAILURE_MSG);
	exit(EXIT_FAILURE);
}

int export_core(t_env **env, char *exp_arg)
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
			export_malloc_failure(*env);
		free(find->value);
		find->value = final;
	}
	else
	{
		find = ft_lstnew_env(exp_arg);
		if(!find)
			export_malloc_failure(*env);
		ft_lstadd_back_env(env, find);
	}
	return (0);
}


int export(t_env **env, t_treenode *export_root)
{
	t_arg *args;

	args = export_root->args;
	if(!args->next)
	{
		ft_lstiter_env(*env, true);
		return (0);
	}
	args = args->next;
	while(args)
	{
		export_core(env, args->content);
		args = args->next;
	}
	return (0);
}