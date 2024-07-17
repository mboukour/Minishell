/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 04:47:41 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/17 13:03:00 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pre_tree.h"

static t_cmd_arg	*get_redir_cmd_arg(t_lex **lex)
{
	t_cmd_arg	*after_head;
	t_cmd_arg	*after_current;
	t_arg		*arg;

	after_head = NULL;
	jump_spaces(lex);
	while ((*lex) && (*lex)->token != AND && (*lex)->token != OR
		&& (*lex)->token != PIPE_LINE && (*lex)->token != CLOSE_PARANTHESE
		&& (*lex)->token != OPEN_PARANTHESE)
	{
		jump_spaces(lex);
		arg = make_args(lex);
		if (!arg)
			break ;
		after_current = ft_lstnew_cmd_arg(arg);
		ft_lstaddback_cmd_arg(&after_head, after_current);
	}
	return (after_head);
}

static int	append_additional_redir_str(t_middle *current,
		t_cmd_arg *after_head)
{
	t_middle	*prev;

	prev = current->prev;
	while (prev && (prev->token == REDIR_OUT || prev->to_replace == DREDIR_OUT))
		prev = prev->prev;
	if (prev && prev->token == COMMAND)
	{
		if (!prev->cmd_arg)
			prev->cmd_arg = after_head;
		else
			prev->cmd_arg->next = after_head;
		return (1);
	}
	return (0);
}

static void	after_command(t_lex **lex, t_middle **head, t_middle *current)
{
	t_cmd_arg	*after_head;
	t_middle	*new_cmd;
	t_arg		*arg;
	t_middle	*prev;

	arg = NULL;
	after_head = get_redir_cmd_arg(lex);
	if (after_head)
	{
		if (!append_additional_redir_str(current, after_head))
		{
			new_cmd = ft_lstnew_middle(after_head->arg,
					after_head->next, COMMAND);
			current->prev = new_cmd;
			new_cmd->next = current;
			(*head) = new_cmd;
		}
	}
}

void	process_redirection_token(t_lex **lex, t_middle **head, int token)
{
	t_middle	*current;
	t_arg		*arg;
	t_arg		*arg_head;
	int			current_jc;

	arg_head = NULL;
	*lex = (*lex)->next;
	jump_spaces(lex);
	current_jc = (*lex)->join_count;
	current = ft_lstnew_middle(NULL, NULL, token);
	while ((*lex) && ((*lex)->token == COMMAND || (*lex)->token == ENV
			|| (*lex)->token == STAR) && (*lex)->join_count == current_jc)
	{
		arg = ft_lstnew_arg(*lex);
		ft_lstaddback_arg(&arg_head, arg);
		(*lex) = (*lex)->next;
	}
	current->redirections = arg_head;
	ft_lstadd_back_middle(head, current);
	if (token != HERE_DOC && token != REDIR_IN)
		after_command(lex, head, current);
}
