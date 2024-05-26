/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabrouk <amabrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 20:05:08 by amabrouk          #+#    #+#             */
/*   Updated: 2024/05/26 20:31:30 by amabrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline.h"

void	first_child(t_treenode *root, t_env **env, t_data *data)
{
	if (fork() == 0)
	{
		close(data->end[0]);
		// dup2(root->data->infile, 0) ; 
		dup2(data->end[1], 1);								// Check here
		// close(root->data->infile);
		close(data->end[1]);
		data->cmd = ft_parsing(root, root->content, *env, data);
		execve(data->path, data->cmd, NULL);
		perror("Execve error");
		exit(EXIT_FAILURE);
	}
	//	else forck
}

// void	second_child(t_treenode *root, t_env **env)
// {
// 	pipe(root->data->end);
// 	if (fork() < 0)
// 	{
// 		dup2(root->data.end[0], 0);
// 	}
// }

void	last_child(t_treenode *root, t_env **env, t_data *data)
{
	if (fork() == 0)
	{
		close(data->end[1]);
		dup2(data->end[0], 0) ;
		//  dup2(data->outfile, 1);														// Check here
		close(data->end[0]);
		// close(data->outfile);
		data->cmd = ft_parsing(root, root->content, *env, data);
		execve(data->path, data->cmd, NULL);
		perror("Execve error");
		exit(EXIT_FAILURE);
	}
	//	else forck
}

void	ft_pipe(t_treenode *root, t_env **env)
{
	int		save_in;
	int 	save_out;
	t_data	data;

	save_in = dup(0);
	save_out = dup(1);
	// data = malloc(sizeof(t_data));
	if (pipe(data.end) == -1)
	{
		fprintf(stderr, "PIPE_ERROR\n");
		return ;
	}
	first_child(root->left, env, &data);
	// close(data->infile);
	// write(2, "\n	yes		\n\n", 9); 
	close(data.end[1]);
	last_child(root->right, env, &data);
	// close(data->outfile);
	close(data.end[0]);
	while (wait(NULL) != -1)
		;
	fprintf(stderr, "FINIHSED\n");
	dup2(save_in, 0);
	dup2(save_out, 1);
}
