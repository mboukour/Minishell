/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_input_checker_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 01:47:49 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 01:58:05 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

bool	is_redir(t_lex *lex)
{
	return (lex->token == REDIR_IN || lex->token == DREDIR_OUT
		|| lex->token == HERE_DOC || lex->token == REDIR_OUT);
}

bool	is_valid_prev_token(t_lex *token)
{
	while (token && token->token == WHITE_SPACE)
		token = token->prev;
	if (!token || (token->token != WORD
			&& token->token != ENV && token->token != STAR
			&& token->token != QUOTE && token->token != DOUBLE_QUOTE
			&& token->token != CLOSE_PARANTHESE))
		return (false);
	return (true);
}

bool	is_valid_next_token(t_lex *token)
{
	while (token && token->token == WHITE_SPACE)
		token = token->next;
	if (!token || (token->token != WORD && token->token != ENV
			&& token->token != STAR && token->token != QUOTE
			&& token->token != DOUBLE_QUOTE && token->token != OPEN_PARANTHESE
			&& !is_redir(token)))
		return (false);
	return (true);
}

t_lex	*check_logical_operators(t_lex *tokens)
{
	if (!is_valid_prev_token(tokens->prev))
		return (tokens);
	return (NULL);
}

t_lex	*check_redirections(t_lex *tokens)
{
	if (!is_valid_next_token(tokens->next) || is_redir(tokens->next))
		return (tokens);
	tokens = skip_sp(tokens, 1);
	if (is_redir(tokens))
		return (tokens);
	return (NULL);
}
