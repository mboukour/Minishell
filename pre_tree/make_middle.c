/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_middle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 04:14:04 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/15 05:59:49 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pre_tree.h"

void	process_other_token(t_lex **lex, t_middle **head)
{
	t_arg		*other;
	int			token;
	t_middle	*current;

	other = ft_lstnew_arg((*lex));
	token = (*lex)->token;
	(*lex) = (*lex)->next;
	jump_spaces(lex);
	current = ft_lstnew_middle(other, NULL, token);
	ft_lstadd_back_middle(head, current);
}

void	handle_not_in_cmd(t_lex **lex, t_middle_vars *vars)
{
	vars->command = make_command(lex);
	if (!(*lex) || (*lex)->token == AND || (*lex)->token == OR
		|| (*lex)->token == PIPE_LINE || is_redir(*lex)
		|| (*lex)->token == CLOSE_PARANTHESE)
	{
		vars->current = ft_lstnew_middle(vars->command, NULL, COMMAND);
		ft_lstadd_back_middle(&vars->head, vars->current);
	}
	vars->in_command = true;
}

void	handle_in_cmd(t_lex **lex, t_middle_vars *vars)
{
	t_arg		*args;
	t_cmd_arg	*cmd_head;
	t_cmd_arg	*cmd_current;

	cmd_head = NULL;
	while ((*lex) && (*lex)->token != AND && (*lex)->token != OR
		&& (*lex)->token != PIPE_LINE && (*lex)->token != CLOSE_PARANTHESE
		&& (*lex)->token != OPEN_PARANTHESE)
	{
		jump_spaces(lex);
		if (!*lex)
			break ;
		args = make_args(lex);
		if (!args)
			break ;
		cmd_current = ft_lstnew_cmd_arg(args);
		ft_lstaddback_cmd_arg(&cmd_head, cmd_current);
	}
	vars->current = ft_lstnew_middle(vars->command, cmd_head, COMMAND);
	ft_lstadd_back_middle(&vars->head, vars->current);
	vars->in_command = false;
}

void	process_word_token(t_lex **lex, t_middle_vars *vars)
{
	if (!vars->in_command)
		handle_not_in_cmd(lex, vars);
	else
		handle_in_cmd(lex, vars);
}

t_middle	*make_middle(t_lex *lex)
{
	t_middle_vars	vars;

	initialize_vars(&vars);
	while (lex)
	{
		if (lex->token == WORD || lex->token == ENV || lex->token == STAR)
			process_word_token(&lex, &vars);
		else if (lex->token == REDIR_IN || lex->token == REDIR_OUT
			|| lex->token == DREDIR_OUT || lex->token == HERE_DOC)
		{
			process_redirection_token(&lex, &vars.head, lex->token);
			vars.in_command = false;
		}
		else if (lex->token != WHITE_SPACE)
		{
			process_other_token(&lex, &vars.head);
			vars.in_command = false;
		}
		else
			jump_spaces(&lex);
	}
	return (vars.head);
}
