/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 00:13:50 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 01:14:00 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../builtins.h"

t_env	*get_env(t_env *env, char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	while (env)
	{
		if (!ft_strncmp(str, env->value, i) && (env->value[i] == '='
				|| (!env->envyable && !env->value[i])))
			return (env);
		env = env->next;
	}
	return (NULL);
}

char	*get_look_for(char *str)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	if (!is_append(str))
		return (str);
	new_str = ft_strdup(str, GC);
	while (str[i])
	{
		if (str[i] != '+')
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

char	*get_append_result(char *exp_arg, char *tmp)
{
	char	*first_part;
	int		i;

	i = 0;
	while (*tmp && *tmp != '=')
		tmp++;
	tmp++;
	while (exp_arg[i] && exp_arg[i] != '=')
		i++;
	i++;
	first_part = ft_substr(exp_arg, 0, i, GC);
	first_part = ft_strjoin(first_part, tmp, GC);
	exp_arg += i;
	return (ft_strjoin(first_part, exp_arg, MANUAL));
}

void	add_new_env(t_env **env, char *exp_arg, int exp_type)
{
	t_env	*new_env;

	new_env = ft_lstnew_env(exp_arg, MANUAL);
	store_malloced(new_env);
	store_malloced(new_env->value);
	ft_lstadd_back_env(env, new_env);
	if (exp_type == 3)
		new_env->envyable = false;
	else
		new_env->envyable = true;
}

void	update_existing_env(t_env *find, char *final, char *exp_arg)
{
	store_malloced(final);
	free(find->value);
	remove_ptr(find->value);
	find->value = final;
	set_joinables(find->value, &find->before_joinable, &find->after_joinable);
	find->envyable = true;
}
