/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 04:31:41 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 05:18:32 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pre_tree.h"

t_arg	*ft_lstnew_arg(t_lex *word)
{
	t_arg	*new_node;

	new_node = smart_malloc(sizeof(t_arg));
	new_node->content = NULL;
	new_node->to_replace = false;
	new_node->join_count = NONE;
	new_node->token = NONE;
	new_node->after_joinable = true;
	new_node->before_joinable = true;
	if (word)
	{
		new_node->content = ft_strdup(word->content, GC);
		new_node->to_replace = word->to_replace;
		new_node->join_count = NONE;
		new_node->token = word->token;
	}
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

int	count_arg(t_arg *arg)
{
	int	i;

	i = 0;
	while (arg)
	{
		i++;
		arg = arg->next;
	}
	return (i);
}

char	**args_to_arr(t_arg *arg)
{
	int		i;
	int		size;
	char	**result;

	i = 0;
	size = count_arg(arg);
	result = smart_malloc((size + 1) * sizeof(char *));
	while (arg)
	{
		result[i] = ft_strdup(arg->content, GC);
		i++;
		arg = arg->next;
	}
	result[i] = NULL;
	return (result);
}

t_arg	*ft_lstlast_arg(t_arg *head)
{
	while (head->next)
		head = head->next;
	return (head);
}

t_arg	*ft_lstfirst_arg(t_arg *tail)
{
	while (tail->prev)
		tail = tail->prev;
	return (tail);
}
