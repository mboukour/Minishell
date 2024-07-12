/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:22:34 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/12 17:47:58 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

void	nullify_all(t_treenode *node)
{
	node->token = NONE;
	node->content = NULL;
	node->args = NULL;
	node->command = NULL;
	node->cmd_arg = NULL;
	node->builtin = NONE;
}

t_arg	*copy_arg(t_arg *arg)
{
	t_arg	*res;
	t_arg	*new;

	res = NULL;
	while (arg)
	{
		new = ft_lstnew_arg(NULL);
		new->content = ft_strdup(arg->content, GC);
		new->to_replace = arg->to_replace;
		new->token = arg->token;
		ft_lstaddback_arg(&res, new);
		arg = arg->next;
	}
	return (res);
}

t_treenode	*new_treenode(t_middle *middled)
{
	t_treenode		*new_node;

	new_node = smart_malloc(sizeof(t_treenode));
	if (middled)
	{
		new_node->token = middled->token;
		new_node->command = copy_arg(middled->command);
		new_node->builtin = middled->builtin;
		new_node->cmd_arg = middled->cmd_arg;
	}
	else
		nullify_all(new_node);
	new_node->content = NULL;
	new_node->args = NULL;
	new_node->after_redir = NULL;
	new_node->before_redir = NULL;
	new_node->is_a_directory = false;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

t_redir	*handle_after_redirs(t_middle **middled)
{
	t_redir	*result;
	t_redir	*current;

	result = NULL;
	while (*middled && is_redir_mid(*middled))
	{
		current = ft_lstnew_redir(*middled);
		ft_lstaddback_redir(&result, current);
		*middled = (*middled)->next;
	}
	return (result);
}

t_redir	*handle_before_redirs(t_middle **middled)
{
	t_redir	*result;
	t_redir	*current;

	result = NULL;
	while (*middled && ((*middled)->token == REDIR_IN
			|| (*middled)->token == HERE_DOC))
	{
		current = ft_lstnew_redir(*middled);
		ft_lstaddback_redir(&result, current);
		*middled = (*middled)->next;
	}
	return (result);
}
