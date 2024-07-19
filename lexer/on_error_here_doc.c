/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_error_here_doc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 02:00:02 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/19 09:45:16 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	fake_readline(char *line, char *delimiter)
{
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, delimiter))
		{
			free(line);
			break ;
		}
		free(line);
	}
}

bool	handle_on_error_heredoc(t_lex *token, t_lex *final)
{
	char	*delimiter;
	int		join_count;
	char	*line;
	int		to_replace;

	line = NULL;
	g_sigint = 0;
	signal(SIGINT, sigint_handler_c);
	delimiter = NULL;
	to_replace = REPLACE_ALL;
	token = skip_sp(token, 1);
	if (token->token != WORD && token->token != ENV && token->token != STAR)
		return (0);
	join_count = token->join_count;
	while ((token->token == WORD || token->token == ENV
			|| token->token == STAR) && token->join_count == join_count)
	{
		if (token == final)
			return (true);
		if (token->to_replace < to_replace)
			to_replace = token->to_replace;
		delimiter = ft_strjoin(delimiter, token->content, GC);
		token = token->next;
	}
	return (fake_readline(line, delimiter), false);
}

void	fake_open(t_lex *lex, t_lex *final)
{
	while (lex)
	{
		if (lex == final || g_sigint)
			break ;
		if (lex->token == HERE_DOC)
		{
			if (handle_on_error_heredoc(lex, final))
				return ;
		}
		lex = lex->next;
	}
}
