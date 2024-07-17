/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_env.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 22:16:27 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/17 08:59:59 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*globalizer_env(int mode, void *env)
{
	static void	*head;

	if (mode == SET)
		head = env;
	else
		return (head);
	return (NULL);
}

void	exceeded_heredoc(t_lex *lex)
{
	int	count;

	count = 0;
	while (lex)
	{
		if (lex->token == HERE_DOC)
			count++;
		if (count > 16)
		{
			ft_putstr_fd(2, "maximum here-document count exceeded\n");
			exit_core(2);
		}
		lex = lex->next;
	}
}

void	exceed_heredoc_syntax_error(t_lex *lex, t_lex *final)
{
	int	count;

	count = 0;
	while (lex)
	{
		if (lex == final)
			break ;
		if (lex->token == HERE_DOC)
			count++;
		if (count > 16)
		{
			ft_putstr_fd(2, "maximum here-document count exceeded\n");
			exit_core(2);
		}
		lex = lex->next;
	}
}
