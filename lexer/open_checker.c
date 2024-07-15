/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_checker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 03:21:19 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/14 23:29:20 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_checker(t_lex *token)
{
	bool	in_quotes;
	bool	in_dquotes;
	bool	in_para;

	in_quotes = false;
	in_dquotes = false;
	in_para = false;
	while (token)
	{
		if (token->token == DOUBLE_QUOTE && !(in_quotes))
			in_dquotes = !(in_dquotes);
		else if (token->token == QUOTE && !(in_dquotes))
			in_quotes = !(in_quotes);
		else if ((token->token == OPEN_PARANTHESE
				|| token->token == CLOSE_PARANTHESE)
			&& !in_quotes && !in_dquotes)
			in_para = !(in_para);
		token = token->next;
	}
	if (in_dquotes || in_quotes)
		return (QUOTE);
	else if (in_para)
		return (OPEN_PARANTHESE);
	return (NONE);
}
