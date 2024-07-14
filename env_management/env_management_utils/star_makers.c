/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   star_makers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 02:26:18 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/14 02:26:32 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../env_management.h"

t_arg	*arg_star_matching(char *to_match)
{
	t_arg	*head;
	t_arg	*current;
	t_env	*star_res;

	head = NULL;
	star_res = star_matching(to_match);
	while (star_res)
	{
		current = ft_lstnew_arg(NULL);
		current->content = ft_strdup(star_res->value, GC);
		ft_lstaddback_arg(&head, current);
		star_res = star_res->next;
	}
	return (head);
}

t_cmd_arg	*cmd_arg_star_matching(char *to_match)
{
	t_cmd_arg	*head_arg;
	t_cmd_arg	*current_arg;
	t_arg		*arg;
	t_env		*env_star;

	head_arg = NULL;
	env_star = star_matching(to_match);
	while (env_star)
	{
		arg = ft_lstnew_arg(NULL);
		arg->content = ft_strdup(env_star->value, GC);
		current_arg = ft_lstnew_cmd_arg(arg);
		ft_lstaddback_cmd_arg(&head_arg, current_arg);
		env_star = env_star->next;
	}
	return (head_arg);
}
