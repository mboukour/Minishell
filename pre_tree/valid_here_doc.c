/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_here_doc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 03:49:11 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 04:18:24 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_heredoc_sigint;

void	sigint_handler_c(int sig)
{
	close(STDIN_FILENO);
	g_heredoc_sigint = sig;
}

char	*get_here_doc_input(t_middle *middle)
{
	char	*line;
	char	*delimiter;
	char	*result;
	char	*tmp;

	result = NULL;
	delimiter = args_to_str(middle->redirections);
	g_heredoc_sigint = 0;
	signal(SIGINT, sigint_handler_c);
	while (true)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (!ft_strcmp(line, delimiter))
		{
			free(line);
			break ;
		}
		tmp = line;
		line = ft_strjoin(line, "\n", GC);
		free(tmp);
		result = ft_strjoin(result, line, GC);
	}
	return (result);
}

void	valid_here_doc(t_middle *middle)
{
	while (middle)
	{
		if (middle->token == HERE_DOC)
			middle->here_doc_buffer = get_here_doc_input(middle);
		middle = middle->next;
	}
}
