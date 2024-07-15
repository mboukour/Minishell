/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 17:28:41 by amabrouk          #+#    #+#             */
/*   Updated: 2024/07/16 00:28:41 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*get_here_doc_path(void)
{
	int		i;
	char	*str;
	char	*num;

	i = 0;
	while (true)
	{
		num = ft_itoa(i, MANUAL);
		null_protector(num);
		str = ft_strjoin("/tmp/.here_doc", num, GC);
		free(num);
		null_protector(str);
		if (access(str, F_OK) == -1)
			return (str);
		free(str);
		i++;
	}
}

int	ft_redir_in(t_redir *redir, t_treenode *root, t_env **env)
{
	int	fd;

	fd = open(redir->redir_string, O_RDONLY, 0777);
	if (fd == -1)
		return (fd_not_open(root, redir, env), 0);
	store_fds(fd);
	if (dup2(fd, 0) == -1)
		return (fd_not_open(root, redir, env), 0);
	change_status(env, 0);
	close(fd);
	remove_fd_node(fd);
	return (1);
}

void	ft_redir_out(t_redir *redir, t_treenode *root, t_env **env)
{
	int	fd;

	fd = NONE;
	if (redir->token == DREDIR_OUT)
		fd = open(redir->redir_string, O_RDWR | O_CREAT | O_APPEND, 0777);
	else if (redir->token == REDIR_OUT)
		fd = open(redir->redir_string, O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd == -1)
	{
		fd_not_open(root, redir, env);
		return ;
	}
	store_fds(fd);
	if (dup2(fd, 1) == -1)
	{
		fd_not_open(root, redir, env);
		return ;
	}
	change_status(env, 0);
	close(fd);
	remove_fd_node(fd);
}

void	ft_heredoc(t_redir *redir, t_treenode *root, t_env **env)
{
	int		fd;
	char	*here_doc_path;

	here_doc_path = get_here_doc_path();
	fd = open(here_doc_path, O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (fd == -1)
		fd_not_open(root, redir, env);
	store_fds(fd);
	if (redir->here_doc_buffer)
		ft_heredoc_buffer(redir, env, fd);
	close(fd);
	remove_fd_node(fd);
	ft_dup_heredoc(root, redir, here_doc_path, env);
}

void	handle_red(t_redir *redir, t_treenode *root, t_env **env)
{
	flag_last_here_doc(redir);
	while (redir)
	{
		if (redir->token == REDIR_IN)
		{
			if (!ft_redir_in(redir, root, env))
				return ;
		}
		else if (redir->token == REDIR_OUT || redir->token == DREDIR_OUT)
			ft_redir_out(redir, root, env);
		else if (redir->token == HERE_DOC)
			ft_heredoc(redir, root, env);
		redir = redir->next;
	}
}
