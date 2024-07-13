/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 18:11:26 by amabrouk          #+#    #+#             */
/*   Updated: 2024/07/13 18:25:12 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	pipeline_error(char *first, int status)
{
	perror(first);
	exit_core(status);
}

int	right_side_of_tree(t_treenode *root, t_data *data, t_env **env)
{
	pid_t	pid2;

	pid2 = fork();
	if (pid2 == -1)
		pipeline_error("fork: ", FORK_ERROR_STATUS);
	else if (pid2 == 0)
	{
		close(data->end[1]);
		if (dup2(data->end[0], 0) == -1)
		{
			close(data->end[0]);
			pipeline_error("dup2: ", EXIT_FAILURE);
		}
		close(data->end[0]);
		traverse_tree(root->right, data, env);
		exit(data->status);
	}
	close(data->end[0]);
	close(data->end[1]);
	return (pid2);
}

void	pipe_error(t_treenode *root, t_env **env)
{
	perror("pipe: ");
	init_tree(root);
	change_status(env, EXIT_FAILURE);
	return ;
}

void	pipeline(t_treenode *root, t_data *data, t_env **env)
{
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(data->end) == -1)
		pipe_error(root, env);
	pid1 = fork();
	if (pid1 == -1)
		pipeline_error("fork: ", FORK_ERROR_STATUS);
	else if (pid1 == 0)
	{
		close(data->end[0]);
		if (dup2(data->end[1], 1) == -1)
		{
			close(data->end[1]);
			pipeline_error("dup2: ", EXIT_FAILURE);
		}
		close(data->end[1]);
		traverse_tree(root->left, data, env);
		exit(data->status);
	}
	pid2 = right_side_of_tree(root, data, env);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &data->status, 0);
	data->status = WEXITSTATUS(data->status);
	change_status(env, data->status);
}
