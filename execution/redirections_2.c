/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 17:35:59 by amabrouk          #+#    #+#             */
/*   Updated: 2024/07/15 02:15:59 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	fd_not_open(t_treenode *root, t_redir *redir, t_env **env)
{
	ft_putstr_fd(2, redir->redir_string);
	perror(": ");
	change_status(env, 1);
	init_tree(root);
	return ;
}

void	ft_heredoc_buffer(t_redir *redir, t_env **env, int fd)
{
	char	*buffer;
	char	*line;

	buffer = redir->here_doc_buffer;
	while (true)
	{
		line = get_line_from_buffer(&buffer);
		if (!line)
			break ;
		line = expanded_line(redir, line, *env);
		if (line)
			write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
}

void	ft_dup_heredoc(t_treenode *root, t_redir *redir, char *p, t_env **env)
{
	int	fd;

	fd = open(p, O_RDONLY);
	if (fd == -1)
		fd_not_open(root, redir, env);
	store_fds(fd);
	if (redir->actual_here_doc)
	{
		if (dup2(fd, 0) == -1)
		{
			ft_putstr_fd(2, "dup2: ");
			perror("");
			change_status(env, 1);
			init_tree(root);
			return ;
		}
	}
	close(fd);
	remove_fd_node(fd);
	unlink(p);
}

char	*get_line_from_buffer(char **buffer)
{
	char	*result;
	int		i;

	i = 0;
	if (!*buffer || **buffer == '\0')
		return (NULL);
	while ((*buffer)[i] && (*buffer)[i] != '\n')
		i++;
	result = ft_substr(*buffer, 0, i, GC);
	if ((*buffer)[i] == '\n')
		i++;
	*buffer += i;
	return (result);
}

void	flag_last_here_doc(t_redir *redir)
{
	t_redir	*last;

	last = ft_lstlast_redir(redir);
	while (last)
	{
		if (last->token == HERE_DOC)
		{
			last->actual_here_doc = true;
			break ;
		}
		last = last->prev;
	}
}
