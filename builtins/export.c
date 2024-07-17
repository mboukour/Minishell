/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 01:20:44 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/17 12:38:12 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	handle_find_case(t_env **env, char *exp_arg, t_env *find, int exp_type)
{
	char	*final;

	if (find)
	{
		if (exp_type == 3)
			return ;
		if (exp_type == 4)
			final = get_append_result(exp_arg, find->value);
		else
			final = ft_strdup(exp_arg, MANUAL);
		update_existing_env(find, final);
	}
	else
		add_new_env(env, exp_arg, exp_type);
}

int	export_core(t_env **env, char *exp_arg)
{
	t_env	*find;
	int		exp_type;

	exp_type = get_export_type(exp_arg, 0);
	if (!exp_type)
		return (export_error(exp_arg), 1);
	else if (exp_type == 2)
		return (0);
	exp_arg = get_look_for(exp_arg);
	find = get_env(*env, exp_arg);
	return (handle_find_case(env, exp_arg, find, exp_type), 0);
}

void	print_env_variable(char *value)
{
	int	i;

	i = 0;
	printf("declare -x ");
	while (value[i] && value[i] != '=')
	{
		printf("%c", value[i]);
		i++;
	}
	if (value[i] && value[i] == '=')
	{
		printf("=\"");
		i++;
		while (value[i])
		{
			printf("%c", value[i]);
			i++;
		}
		printf("\"\n");
	}
	else
		printf("\n");
}

void	export_no_arg(t_env *env)
{
	env = copy_env(env);
	sort_env_list(env);
	while (env)
	{
		if (ft_strncmp(env->value, "?=", 2) && ft_strncmp(env->value, "_=", 2))
			print_env_variable(env->value);
		env = env->next;
	}
}

int	export(t_env **env, t_treenode *export_root)
{
	t_arg	*args;
	int		ret;

	ret = 0;
	args = export_root->args;
	if (!args)
		return (export_no_arg(*env), 0);
	while (args)
	{
		if (args->content[0] == '?' && args->content[1] == '=')
		{
			export_error(args->content);
			ret = 1;
		}
		else
			ret = export_core(env, args->content);
		args = args->next;
	}
	return (ret);
}
