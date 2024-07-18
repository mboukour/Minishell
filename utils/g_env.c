/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_env.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 22:16:27 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/18 16:15:06 by mboukour         ###   ########.fr       */
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
int	get_latest_status(t_env *env)
{
	char	*value;
	t_env	*q_env;

	q_env = get_env(env, "?");
	return (ft_atoi(q_env->value + 2, NULL));
}