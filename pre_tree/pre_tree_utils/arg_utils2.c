/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 04:32:18 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 05:18:30 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pre_tree.h"

int	ft_lstsize_arg(t_arg *arg)
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

void	ft_lstaddback_arg(t_arg **head, t_arg *new)
{
	t_arg	*last_arg;

	if (!*head)
	{
		*head = new;
		return ;
	}
	last_arg = ft_lstlast_arg(*head);
	last_arg->next = new;
	new->prev = last_arg;
}

void	ft_lstiter_arg(t_arg *arg)
{
	while (arg)
	{
		printf("%s:%i\n", arg->content, arg->to_replace);
		arg = arg->next;
	}
}
