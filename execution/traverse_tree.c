/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traverse_tree.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabrouk <amabrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 07:51:20 by amabrouk          #+#    #+#             */
/*   Updated: 2024/06/06 15:56:11 by amabrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void sigint_handler_cmd(int sig)
{
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
}

void expand_args(t_arg *args, t_env *env)
{
	while(args)
	{
		if(args->to_replace != NO_REPLACE)
		{
				args->content = env_expander(args->content, env);
			if(args->to_replace == REPLACE_ALL)
				args->content = star_matching(args->content);
		}
		args = args->next;
	}
}

void expand_redirs(t_redir *redir, t_env *env)
{
	while (redir)
	{
		if(redir->to_replace != NO_REPLACE)
		{
			redir->redir_string = env_expander(redir->redir_string, env);
			if(redir->to_replace == REPLACE_ALL)
				redir->redir_string = star_matching(redir->redir_string);
		}
		redir = redir->next;
	}
}

void expand_node(t_treenode *root, t_env *env)
{
	if(root->content && root->to_replace != NO_REPLACE)
	{
		root->content = env_expander(root->content, env);
		if(root->to_replace == REPLACE_ALL)
			root->content = star_matching(root->content);
	}
	expand_args(root->args, env);
	expand_redirs(root->before_redir, env);
	expand_redirs(root->after_redir, env);
}


int	traverse_tree(t_treenode *root, t_data *data, t_env **env)
{
	int		save_in;
	int		save_out;

	save_in = dup(0);
	save_out = dup(1);
	signal(SIGINT, sigint_handler_cmd);
	if (!root)
		return (0);
	if(root->token != AND  && root->token != OR && root->token != PIPE_LINE)
		expand_node(root, *env);
	if (root->before_redir)
	{
		handle_red(root->before_redir);
		printf("before\n");
	}
	if (root->after_redir)
	{
		handle_red(root->after_redir);
		printf("after\n");
	}
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
	else
		fprintf(stderr, "ERROR: %i\n", root->token);
	dup2(save_in, 0);
	dup2(save_out, 1);
	close(save_in);
	close(save_out);
	return 1;
}
