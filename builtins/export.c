#include "../minishell.h"


int get_export_type(char *str)
{
    int i;

	i = 0;
	if(str[0] == '?' && str[1] == '=')
		return(1);
	if(str[0] == '_' && str[1] == '=')
		return(1);
	while(str[i] && str[i] != '=')
	{
		if(ft_isalpha(str[i]))
			break;
		i++;
	}
	if(str[i] && str[i] == '=')
		return (0);
	if(!str[i] || (str[i] == '=' && !str[i + 1]))
		return (2);
	return (1);
}


void export_error(char *str)
{
	ft_putstr_fd(2, "export: `");
	ft_putstr_fd(2, str);
	ft_putstr_fd(2, "' : not a valid identifier\n");
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
	bool before_joinable;
	bool after_joinable;
	int exp_type;
	int i;

	i = 0;
	exp_type = get_export_type(exp_arg);
	if(!exp_type)
	{
		export_error(exp_arg);
		return (1);
	}
	if(exp_type == 2)
		return (0);
	find = get_env(*env, exp_arg);
	if(find)
	{
		set_joinables(exp_arg, &before_joinable, &after_joinable);
		final = ft_strdup(exp_arg, MANUAL);
		if(!final)
			export_malloc_failure(*env);
		free(find->value);
		find->value = final;
	
		find->before_joinable = before_joinable;
		find->after_joinable = after_joinable;
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
	int status;
	t_arg *args;
	int ret;
	bool error;

	ret = 0;
	error = false;
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
			export_error(args->content);
			error = true;
		}
		else
		{
			ret = export_core(env, args->content);
			if(ret)
				error = true;
		}
		args = args->next;
	}
	if(error)
		return (1);
	return (0);
}