/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 18:14:33 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/17 11:15:17 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../env_management.h"

void	ft_lstiter_env(t_env *env)
{
	while (env)
	{
		if (ft_strncmp(env->value, "?=", 2) && env->envyable)
			printf("%s\n", env->value);
		env = env->next;
	}
}

void	ft_lstclear_env(t_env *head)
{
	t_env	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		if (temp->value)
		{
			free(temp->value);
			temp->value = NULL;
		}
		free(temp);
		temp = NULL;
	}
}

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

t_env	*strict_get_env(t_env *env, char *str)
{
	int	i;
	int	str_len;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	str_len = i;
	while (env)
	{
		if (!ft_strncmp(str, env->value, str_len)
			&& env->value[str_len] == '=' && str[str_len] == '\0')
		{
			if (str_len + 1 == ft_strlen(env->value))
				return (NULL);
			return (env);
		}
		env = env->next;
	}
	return (NULL);
}

void	empty_env_with_args(t_treenode *root)
{
	root->command = root->args;
	root->content = root->command->content;
	root->args = root->args->next;
	if (root->args != NULL)
		root->args->prev = NULL;
	root->command->next = NULL;
}
