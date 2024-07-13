/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   makers.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 04:52:43 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 04:59:20 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pre_tree.h"

t_arg	*make_args(t_lex **first_arg)
{
	t_arg	*args;
	t_arg	*new_arg;
	int		current_jc;

	if (!(*first_arg))
		return (NULL);
	current_jc = (*first_arg)->join_count;
	args = NULL;
	while (((*first_arg)->token == WORD || (*first_arg)->token == ENV
			|| (*first_arg)->token == STAR)
		&& (*first_arg)->join_count == current_jc)
	{
		jump_spaces(first_arg);
		new_arg = ft_lstnew_arg(*first_arg);
		ft_lstaddback_arg(&args, new_arg);
		*first_arg = (*first_arg)->next;
		if (!(*first_arg))
			break ;
	}
	return (args);
}

t_arg	*make_command(t_lex **lex)
{
	t_arg	*head;
	t_arg	*current;
	int		current_jc;

	head = NULL;
	current_jc = (*lex)->join_count;
	while ((*lex) && is_joinable_lex(*lex) && (*lex)->join_count == current_jc)
	{
		current = ft_lstnew_arg((*lex));
		ft_lstaddback_arg(&head, current);
		(*lex) = (*lex)->next;
	}
	jump_spaces(lex);
	return (head);
}
