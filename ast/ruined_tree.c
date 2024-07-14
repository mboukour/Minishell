/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ruined_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:26:52 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/12 17:48:53 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

t_treenode	*parse_command(t_middle **middled)
{
	t_redir		*before_redir;
	t_treenode	*l_node;
	bool		command_flag;

	command_flag = false;
	if ((*middled)->token == OPEN_PARANTHESE)
		return (*middled = (*middled)->next,
			l_node = parse_cmdline(middled), l_node);
	before_redir = handle_before_redirs(middled);
	if (valid_parse_cmd_middle(*middled))
	{
		command_flag = true;
		l_node = new_treenode(*middled);
	}
	else
		l_node = new_treenode(NULL);
	l_node->before_redir = before_redir;
	if (*middled)
	{
		if (command_flag)
			*middled = (*middled)->next;
		l_node->after_redir = handle_after_redirs(middled);
	}
	return (l_node);
}

// <pipeline>  ::= <command> {"|" <command>}
t_treenode	*parse_pipeline(t_middle **middled)
{
	t_treenode	*l_node;
	t_treenode	*r_node;
	t_treenode	*pipe;

	pipe = NULL;
	l_node = parse_command(middled);
	while (*middled && (*middled)->token == PIPE_LINE)
	{
		pipe = new_treenode(*middled);
		*middled = (*middled)->next;
		r_node = parse_pipeline(middled);
		pipe->left = l_node;
		pipe->right = r_node;
	}
	if (pipe)
		return (pipe);
	return (l_node);
}

t_treenode	*parse_cmdline(t_middle **middled)
{
	t_treenode	*l_node;
	t_treenode	*op;
	t_treenode	*r_node;

	op = NULL;
	l_node = parse_pipeline(middled);
	while (*middled && ((*middled)->token == AND || (*middled)->token == OR))
	{
		op = new_treenode(*middled);
		*middled = (*middled)->next;
		r_node = parse_pipeline(middled);
		op->left = l_node;
		op->right = r_node;
		l_node = op;
	}
	if (*middled && (*middled)->token == CLOSE_PARANTHESE)
		*middled = (*middled)->next;
	if (op)
		return (op);
	return (l_node);
}

t_treenode	*ruined_tree(t_middle *middled)
{
	t_treenode	*root;

	root = parse_cmdline(&middled);
	return (root);
}
