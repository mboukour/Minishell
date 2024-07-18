/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 00:13:50 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/18 20:35:35 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../builtins.h"

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

void	update_existing_env(t_env *find, char *final)
{
	store_malloced(final);
	free(find->value);
	remove_ptr(find->value);
	find->value = final;
	set_joinables(find->value, &find->before_joinable, &find->after_joinable);
	find->envyable = true;
}

void	restore_show_pwds(t_data *data, char *content)
{
	if (!ft_strncmp("PWD=", content, 4)
		|| (!ft_strncmp("PWD+=", content, 4))
		|| (!ft_strcmp("PWD", content) && ft_strlen(content) == 3)
	)
		data->show_pwd = true;
	else if (!ft_strncmp("OLDPWD=", content, 7)
		|| (!ft_strcmp("OLDPWD", content) && ft_strlen(content) == 7)
		|| !ft_strncmp("OLDPWD+=", content, 4))
		data->show_oldpwd = true;
}
