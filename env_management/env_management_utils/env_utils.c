/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 17:37:05 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/18 18:08:37 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../env_management.h"

void	set_joinables(char *str, bool *before_joinable, bool *after_joinable)
{
	int	i;

	i = 0;
	*before_joinable = true;
	*after_joinable = true;
	while (str[i] && str[i] != '=')
		i++;
	if (!str[i])
		return ;
	i++;
	if (str[i] && is_ws(str[i]))
		*before_joinable = false;
	while (str[i])
		i++;
	i--;
	if (is_ws(str[i]))
		*after_joinable = false;
}

char	*new_shlvl(char *new_lvl)
{
	char	*new_shlvl;
	char	*lvl;
	int		number;

	number = ft_atoi(new_lvl + 6, NULL);
	if (number < 0)
		lvl = ft_strdup("0", MANUAL);
	else if (number == 999)
		lvl = ft_strdup("", MANUAL);
	else
		lvl = ft_itoa(number + 1, MANUAL);
	null_protector(lvl);
	new_shlvl = ft_strjoin("SHLVL=", lvl, MANUAL);
	null_protector(new_shlvl);
	free(lvl);
	return (new_shlvl);
}

void	create_new_env(t_env **head, char *value, bool *set_shlvl)
{
	t_env	*current;
	char	*new;

	current = ft_lstnew_env(value, MANUAL);
	store_malloced(current);
	store_malloced(current->value);
	if (!ft_strncmp(current->value, "SHLVL", 5) && current->value[5] == '=')
	{
		*set_shlvl = true;
		new = new_shlvl(current->value);
		store_malloced(new);
		free(current->value);
		remove_ptr(current->value);
		current->value = new;
	}
	ft_lstadd_back_env(head, current);
}

t_env	*array_to_env(char **env)
{
	t_env	*head;
	int		i;
	bool	set_shlvl;

	i = 0;
	head = NULL;
	set_shlvl = false;
	while (env[i])
	{
		create_new_env(&head, env[i], &set_shlvl);
		i++;
	}
	if (!set_shlvl)
		exp_w_null(&head, "SHLVL=1");
	return (head);
}

char	**env_to_array(t_env *env)
{
	char	**arr;
	int		i;

	i = 0;
	arr = smart_malloc((ft_lstsize_env(env)) * sizeof(char *));
	while (env)
	{
		if ((env->value[0] != '?' || env->value[1] != '=') && env->envyable)
		{
			arr[i] = ft_strdup(env->value, GC);
			i++;
		}
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}
