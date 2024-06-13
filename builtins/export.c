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
	if(!is_valid_export(exp_arg))
	{
		fprintf(stderr, "Not valid\n");
		return (NONE);
	}
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

// EXPORT WITH NO ARGS PRINTS INSIDE ""
// EXPORT WITH NO ARGS PRINTS IN ALPHABETIC ORDER
// EXPORT NEEDS ITS OWN PARSING
int export(t_env **env, t_treenode *export_root)
{
	int status;
	t_arg *args;

	args = export_root->args;
	if(!args)
	{
		ft_lstiter_env(*env, true);
		return (0);
	}
	while(args)
	{
		if(args->content[0] == '?' && args->content[1] == '=')
		{
			ft_putstr_fd(2, "export: `");
			ft_putstr_fd(2,args->content);
			ft_putstr_fd(2, "' : not a valid identifier\n");
		}
		else
			export_core(env, args->content);
		args = args->next;
	}
	return (0);
}