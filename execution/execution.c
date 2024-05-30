/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabrouk <amabrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 20:05:08 by amabrouk          #+#    #+#             */
/*   Updated: 2024/05/30 15:56:57 by amabrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	first_child(t_treenode *root, t_env **env, t_data *data)
{
	if (fork() == 0)
	{
		close(data->end[0]);
		dup2(data->end[1], 1);
		close(data->end[1]);
		ft_parsing(root, *env, data);
		data->cmd = args_to_arr(root->args);
		execve(data->path, data->cmd, NULL);
		perror("Execve error");
		exit(EXIT_FAILURE);
	}
}


void	last_child(t_treenode *root, t_env **env, t_data *data)
{
	if (fork() == 0)
	{
		close(data->end[1]);
		dup2(data->end[0], 0);												// Check here
		close(data->end[0]);
		ft_parsing(root, *env, data);
		data->cmd = args_to_arr(root->args);
		execve(data->path, data->cmd, NULL);
		perror("Execve error");
		exit(EXIT_FAILURE);
	}
	//	else fork
}
void	pipeline(t_treenode *root, t_env **env)
{
	int		save_in;
	int 	save_out;
	t_data	data;

	if (root->left != NULL)
	{
		save_in = dup(0);
		save_out = dup(1);
		pipe(data.end);
		first_child(root->left, env, &data);
		close(data.end[1]);
		dup2(data.end[0], 0);
		close(data.end[0]);
		pipeline(root->right, env);
		while (wait(NULL) != -1)
			;
		dup2(save_in, 0);
		dup2(save_out, 1);
		close(save_in);
		close(save_out);
	}
	else
	{
		last_child(root, env, &data);
	}
	fprintf(stderr, "FINISHED\n");
}
