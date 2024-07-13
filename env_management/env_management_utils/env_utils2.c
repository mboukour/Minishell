/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 18:14:33 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 18:15:00 by mboukour         ###   ########.fr       */
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
