/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   little_env_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 18:36:34 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/17 00:28:51 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../env_management.h"

bool	is_env(char *str)
{
	int	i;

	if (str[0] != '$' || !str[1])
		return (false);
	i = 1;
	while (str[i])
	{
		if (is_special(str[i]))
			return (false);
		i++;
	}
	return (true);
}

char	*get_real_env(char *value)
{
	int	i;

	i = 0;
	while (value[i] && value[i] != '=')
		i++;
	i++;
	return (&value[i]);
}

t_arg	*env_to_arg(t_env *env_node)
{
	t_arg	*head;
	t_arg	*current;
	char	**sp_res;
	int		i;

	head = NULL;
	i = 0;
	sp_res = ft_split_ws(get_real_env(env_node->value), GC);
	while (sp_res[i])
	{
		current = ft_lstnew_arg(NULL);
		current->content = ft_strdup(sp_res[i], GC);
		current->after_joinable = env_node->after_joinable;
		current->before_joinable = env_node->before_joinable;
		current->to_replace = env_node->star_to_replace;
		ft_lstaddback_arg(&head, current);
		i++;
	}
	return (head);
}

int	after_env_star(char *str)
{
	int		i;
	char	*make_sure;

	i = 0;
	while (str[i] && str[i] != '.' && str[i] != '*'
		&& str[i] != '/' && str[i] != '=')
		i++;
	if (!str[i])
		return (NONE);
	make_sure = ft_substr(str, 0, i, GC);
	if (!is_env(make_sure))
		return (NONE);
	return (i);
}

bool	am_i_a_star(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '*')
			return (true);
		i++;
	}
	return (false);
}
