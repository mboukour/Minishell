/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 18:13:16 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 18:14:58 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*ft_lstnew_env(char *env, int mode)
{
	t_env	*new_node;

	if (mode == MANUAL)
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		new_node->value = ft_strdup(env, MANUAL);
		if (!new_node->value)
			return (free(new_node), NULL);
	}
	else
	{
		new_node = smart_malloc(sizeof(t_env));
		new_node->value = ft_strdup(env, GC);
	}
	set_joinables(env, &new_node->before_joinable, &new_node->after_joinable);
	new_node->star_to_replace = REPLACE_ALL;
	new_node->envyable = true;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

t_env	*ft_lstlast_env(t_env *lst)
{
	t_env	*temp;

	if (lst == NULL)
		return (NULL);
	temp = lst;
	while (temp)
	{
		if (!temp->next)
			return (temp);
		temp = temp->next;
	}
	return (lst);
}

void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*last_env;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last_env = ft_lstlast_env(*lst);
	last_env->next = new;
	new->prev = last_env;
}

int	ft_lstsize_env(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

t_env	*copy_env(t_env *env)
{
	t_env	*head;
	t_env	*current;

	head = NULL;
	while (env)
	{
		current = ft_lstnew_env(env->value, GC);
		ft_lstadd_back_env(&head, current);
		env = env->next;
	}
	return (head);
}
