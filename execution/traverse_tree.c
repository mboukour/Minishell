/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traverse_tree.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 07:51:20 by amabrouk          #+#    #+#             */
/*   Updated: 2024/07/16 03:53:26 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	sigint_handler_cmd(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	sigquit_handler_cmd(int sig)
{
	(void)sig;
	write(1, "Quit\n", 6);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	dups_error(char *str, t_env **env)
{
	perror(str);
	change_status(env, 1);
}

void	traverse_tree_p2(t_treenode *root, t_data *data, t_env **env)
{
	if (root->before_redir)
		handle_red(root->before_redir, root, env);
	if (root->after_redir)
		handle_red(root->after_redir, root, env);
	if (root->token == COMMAND)
		execute_command(root, env, data);
	else if (root->token == PIPE_LINE)
		pipeline(root, data, env);
	else if (root->token == AND)
	{
		if (traverse_tree(root->left, data, env) == 0 || data->status == 0)
			traverse_tree(root->right, data, env);
	}
	else if (root->token == OR)
	{
		if (traverse_tree(root->left, data, env) == 0 || data->status != 0)
			traverse_tree(root->right, data, env);
	}
}

int	traverse_tree(t_treenode *root, t_data *data, t_env **env)
{
	int	save_in;
	int	save_out;

	save_in = dup(0);
	if (save_in == -1)
		return (dups_error("dup:", env), 1);
	save_out = dup(1);
	if (save_in == -1)
		return (close(save_in), dups_error("dup:", env), 1);
	signal(SIGINT, sigint_handler_cmd);
	signal(SIGQUIT, sigquit_handler_cmd);
	if (!root)
		return (0);
	if (root->token != AND && root->token != OR && root->token != PIPE_LINE)
		expand_node(root, env);
	traverse_tree_p2(root, data, env);
	dup2(save_in, 0);
	dup2(save_out, 1);
	close(save_in);
	close(save_out);
	return (1);
}
