/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_input_checker.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 01:49:14 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/17 09:20:16 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	false_syntax(t_bool_syntax *syntax)
{
	syntax->in_dquote = false;
	syntax->in_quote = false;
	syntax->in_para = false;
}

t_lex	*skip_sp(t_lex *lex, int mode)
{
	if (mode == 0)
	{
		lex = lex->prev;
		while (lex && lex->token == WHITE_SPACE)
			lex = lex->prev;
		return (lex);
	}
	else
	{
		lex = lex->next;
		while (lex && lex->token == WHITE_SPACE)
			lex = lex->next;
		return (lex);
	}
}

t_lex	*check_parentheses(t_lex *tokens)
{
	t_lex	*next;
	t_lex	*prev;

	next = skip_sp(tokens, 1);
	prev = skip_sp(tokens, 0);
	if (tokens->token == OPEN_PARANTHESE)
	{
		if (prev && (prev->token != AND && prev->token != OR
				&& prev->token != PIPE_LINE && prev->token != OPEN_PARANTHESE))
			return (tokens);
		if (!next)
			return (tokens);
		else if (next->token == CLOSE_PARANTHESE)
			return (next);
	}
	else if (tokens->token == CLOSE_PARANTHESE)
	{
		if (next && (next->token != AND && next->token != OR
				&& next->token != PIPE_LINE && next->token != CLOSE_PARANTHESE))
			return (tokens);
	}
	return (NULL);
}

t_lex	*check_token(t_lex *tokens, t_bool_syntax *syntax)
{
	t_lex	*error_token;

	error_token = NULL;
	if ((tokens->token == AND || tokens->token == OR
			|| tokens->token == PIPE_LINE)
		&& (!syntax->in_quote && !syntax->in_dquote))
		error_token = check_logical_operators(tokens);
	else if ((tokens->token == REDIR_IN || tokens->token == REDIR_OUT
			|| tokens->token == DREDIR_OUT || tokens->token == HERE_DOC)
		&& (!syntax->in_quote && !syntax->in_dquote))
		error_token = check_redirections(tokens);
	else if (tokens->token == OPEN_PARANTHESE
		|| tokens->token == CLOSE_PARANTHESE)
	{
		if (!syntax->in_quote && !syntax->in_dquote)
			error_token = check_parentheses(tokens);
	}
	return (error_token);
}

t_lex	*lex_input_checker(t_lex *tokens)
{
	t_bool_syntax	syntax;
	t_lex			*error_token;

	false_syntax(&syntax);
	while (tokens)
	{
		error_token = check_token(tokens, &syntax);
		if (error_token)
			return (error_token);
		tokens = tokens->next;
	}
	return (NULL);
}
