/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   little_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 04:48:32 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 04:59:17 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pre_tree.h"

bool	is_joinable_lex(t_lex *lex)
{
	return (lex->token != AND && lex->token != OR
		&& lex->token != PIPE_LINE && lex->token != CLOSE_PARANTHESE);
}

void	jump_spaces(t_lex **lex)
{
	while (*lex && (*lex)->token == WHITE_SPACE)
		(*lex) = (*lex)->next;
}

void	initialize_vars(t_middle_vars *vars)
{
	vars->head = NULL;
	vars->command = NULL;
	vars->current = NULL;
	vars->in_command = false;
}
