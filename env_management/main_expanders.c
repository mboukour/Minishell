/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_expanders.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 01:29:40 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/14 01:39:21 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_management.h"

static t_arg	*expand_args(t_cmd_arg *cmd_arg, t_env *env)
{
	t_arg	*args;

	ultimate_expander(&cmd_arg, env);
	args = final_args(cmd_arg);
	return (args);
}

static void	expand_redirs(t_redir *redir, t_env **env, t_treenode *root)
{
	t_cmd_arg	*for_redir;
	t_arg		*arg;

	while (redir)
	{
		if (redir->token != HERE_DOC)
		{
			for_redir = ft_lstnew_cmd_arg(redir->redir_input);
			arg = expand_args(for_redir, *env);
			expand_arg_as_star(&arg);
			if (!arg || arg->next)
			{
				ambiguous_redirect(redir->redir_input, root, env);
				return ;
			}
			redir->redir_string = args_to_str(arg);
			redir->here_doc_replacer = get_least_replace(redir->redir_input);
		}
		else
		{
			redir->redir_string = args_to_str(redir->redir_input);
			redir->here_doc_replacer = get_least_replace(redir->redir_input);
		}
		redir = redir->next;
	}
}

static void	expand_command(t_treenode *root, t_arg **tmp_arg)
{
	char	*no_star;

	*tmp_arg = NULL;
	no_star = no_stars(root->content);
	expand_arg_as_star(&root->command);
	root->content = root->command->content;
	no_star = no_stars(root->content);
	if (no_star && is_path(no_star) && is_a_directory(no_star))
	{
		root->is_a_directory = true;
		root->content = ft_strdup(no_star, GC);
	}
	*tmp_arg = root->command->next;
	root->command->next = NULL;
	root->args = *tmp_arg;
	ft_lstiter_arg(root->args);
}

void	expand_node(t_treenode *root, t_env **env)
{
	char		*no_star;
	t_arg		*tmp_arg;
	t_cmd_arg	*for_command;
	bool		no_first;

	no_first = false;
	tmp_arg = NULL;
	for_command = ft_lstnew_cmd_arg(root->command);
	root->command = expand_args(for_command, *env);
	if (root->command)
		expand_command(root, &tmp_arg);
	if (!tmp_arg)
		root->args = expand_args(root->cmd_arg, *env);
	else
	{
		tmp_arg->next = expand_args(root->cmd_arg, *env);
		root->args = tmp_arg;
	}
	expand_arg_as_star(&root->args);
	if(!root->command && root->args)
	{
    	root->command = root->args;
		root->content = root->command->content;
		root->args = root->args->next;
		if (root->args != NULL)
			root->args->prev = NULL;
    	root->command->next = NULL;
	}
	else
		export_core(env, "?=0");
	expand_redirs(root->before_redir, env, root);
	expand_redirs(root->after_redir, env, root);
}

void	star_expander(t_cmd_arg **cmd_arg)
{
	t_cmd_arg	*set_cmd;
	t_cmd_arg	*expanded_star;
	t_arg		*arg;
	t_exp_vars	vars;

	vars.looping_cmd = *cmd_arg;
	while (vars.looping_cmd)
	{
		vars.next_lp_cmd = vars.looping_cmd->next;
		arg = vars.looping_cmd->arg;
		while (arg)
		{
			if (is_arg_star(arg) && handle_arg_star(&vars, cmd_arg, arg))
				break ;
			arg = arg->next;
		}
		vars.looping_cmd = vars.next_lp_cmd;
	}
}
