#include "../minishell.h"


bool no_equal(char *str)
{
	int i;

	i = 0;
	while(str[i])
	{
		if(str[i] == '=')
			return(false);
		i++;
	}
	return(true);
}
bool underscore_before_equal(char *str)
{
	int i;

	i = 0;
	while (str[i] && str[i] != '=')
	{
		if(str[i] != '_')
			break;
		i++;
	}
	if(str[i] && str[i] == '=')
		return (true);
	return(false);
}

int get_export_type(char *str)
{
    int i;

	i = 0;
	if(str[0] == '?' && str[1] == '=')
		return(1);
	if(underscore_before_equal(str))
		return(1);
	if(str[0] == '-' && str[1] == '-')
		return(0);
	if(is_c_num(str[0]))
		return(0);
	if(no_equal(str))
		return(3);
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

	i = 0;
	while(str[i] && str[i] != '=')
		i++;
	while(env)
	{
		if(!ft_strncmp(str, env->value, i) && (env->value[i] == '=' || (!env->envyable && !env->value[i])))
				return (env);
		env = env->next;
	}
	return (NULL);
}


int export_core(t_env **env, char *exp_arg)
{
	t_env *find;
	char *final;
	bool before_joinable;
	bool after_joinable;
	int exp_type;

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
		if(exp_type == 3)
			return (0);
		set_joinables(exp_arg, &before_joinable, &after_joinable);
		final = ft_strdup(exp_arg, MANUAL);
		store_malloced(final);
		free(find->value);
		remove_ptr(find->value);
		find->value = final;
		find->before_joinable = before_joinable;
		find->after_joinable = after_joinable;
		find->envyable = true;
	}
	else
	{
		find = ft_lstnew_env(exp_arg, MANUAL);
		store_malloced(find->value);
		store_malloced(find);
		ft_lstadd_back_env(env, find);
		if(exp_type == 3)
			find->envyable = false;
		else
			find->envyable = true;
	}
	return (0);
}

void export_no_arg(t_env *env)
{
	int i;

	env = copy_env(env);
	sort_env_list(env);
	while (env)
	{
		if(ft_strncmp(env->value, "?=", 2) && ft_strncmp(env->value, "_=", 2))
		{
			printf("declare -x ");
			i = 0;
			while (env->value[i] && env->value[i] != '=')
			{
				printf("%c", env->value[i]);
				i++;
			}
			if(env->value[i] && env->value[i] == '=')
			{
				printf("=\"");
				i++;
				while(env->value[i])
				{
					printf("%c", env->value[i]);
					i++;
				}
				printf("\"\n");
			}
			else
				printf("\n");
		}
		env = env->next;
	}
}

int export(t_env **env, t_treenode *export_root)
{
	t_arg *args;
	int ret;
	bool error;

	ret = 0;
	error = false;
	args = export_root->args;
	if(!args)
	{
		export_no_arg(*env);
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