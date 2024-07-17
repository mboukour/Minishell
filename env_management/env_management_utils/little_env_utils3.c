/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   little_env_utils3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 18:39:16 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/17 16:19:35 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../env_management.h"

bool	is_nextable(t_arg *arg, t_env *env)
{
	t_env	*env_node;

	env_node = strict_get_env(env, arg->content);
	if (!env_node)
		return (true);
	else if (env_node->before_joinable)
		return (true);
	return (false);
}

char	*lex_to_str(t_lex *lex)
{
	char	*result;

	result = NULL;
	while (lex)
	{
		result = ft_strjoin(result, lex->content, GC);
		lex = lex->next;
	}
	return (result);
}

void	handle_splitted_env(t_cmd_arg **head, char *value, t_env *env_node)
{
	t_cmd_arg	*current;
	t_arg		*one;

	one = ft_lstnew_arg(NULL);
	one->content = ft_strdup(value, GC);
	one->to_replace = env_node->star_to_replace;
	current = ft_lstnew_cmd_arg(one);
	ft_lstaddback_cmd_arg(head, current);
}
