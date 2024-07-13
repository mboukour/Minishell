/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   little_env_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 18:38:01 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 18:38:55 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../env_management.h"

int get_to_replace(t_arg *arg)
{
	int	to_replace;

	to_replace = REPLACE_ALL;
	while(arg)
	{
		if(is_arg_star(arg) && arg->to_replace < to_replace)
			to_replace = arg->to_replace;
		arg = arg->next;
	}
	return (to_replace);
}

char *args_to_str(t_arg *args)
{
	char	*result;

	result = NULL;
	while (args)
	{
		result = ft_strjoin(result, args->content, GC);
		args = args->next;
	}
	return (result);
}

int get_least_replace(t_arg *args)
{
	int	to_replace;

	to_replace = REPLACE_ALL;
	while (args)
	{
		if(args->to_replace < to_replace)
			to_replace = args->to_replace;
		args = args->next;
	}
	return (to_replace);
}
void expand_arg_as_star(t_arg **head)
{
	t_arg *arg;
	t_arg *tmp_arg;
	t_arg *next;

	arg = *head;
	while(arg && (arg->to_replace == REPLACE_ALL))
	{
		next = arg->next;
		tmp_arg = arg_star_matching(arg->content);
		if(tmp_arg)
		{
			replace_arg_with_list(head, arg, tmp_arg);
			next = tmp_arg->next;
		}
		arg = next;
	}
}
bool is_arg_star(t_arg *arg)
{
	int i;
	char *str;

	i = 0;
	if (arg->to_replace != REPLACE_ALL)
		return (false);
	str = arg->content;
	while (str[i])
	{
		if(str[i] == '*')
			return (true);
		i++;
	}
	return (false);
}
