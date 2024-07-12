/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:23:23 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/12 17:47:53 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

t_redir	*ft_lstnew_redir(t_middle *middled)
{
	t_redir	*new_node;

	new_node = smart_malloc(sizeof(t_redir));
	new_node->token = (middled)->token;
	new_node->redir_string = NULL;
	new_node->redir_input = middled->redirections;
	new_node->to_replace = middled->to_replace;
	new_node->here_doc_replacer = REPLACE_ALL;
	new_node->actual_here_doc = false;
	new_node->here_doc_buffer = middled->here_doc_buffer;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

t_redir	*ft_lstlast_redir(t_redir *first)
{
	while (first)
	{
		if (!first->next)
			return (first);
		first = first->next;
	}
	return (NULL);
}

void	ft_lstaddback_redir(t_redir **head, t_redir *new)
{
	t_redir	*last;

	if (!head)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	last = ft_lstlast_redir(*head);
	last->next = new;
	new->prev = last;
}

bool	is_redir_mid(t_middle *middled)
{
	return (middled->token == REDIR_OUT || middled->token == DREDIR_OUT
		|| middled->token == HERE_DOC || middled->token == REDIR_IN);
}

bool	valid_parse_cmd_middle(t_middle *middled)
{
	return (middled && ((middled)->token != REDIR_OUT
			&& (middled)->token != DREDIR_OUT) && (middled)->token != AND
		&& (middled)->token != OR && (middled)->token != PIPE_LINE);
}

// char	*redir_to_str(int token)
// {
// 	if (token == DREDIR_OUT)
// 		return (">>");
// 	else if (token == REDIR_OUT)
// 		return (">");
// 	else if (token == REDIR_IN)
// 		return ("<");
// 	else
// 		return ("<<");
// }

// void	ft_lstiter_redir(t_redir *first)
// {
// 	t_arg	*arg;

// 	while (first)
// 	{
// 		arg = first->redir_input;
// 		printf("(%s: ", redir_to_str(first->token));
// 		while (arg)
// 		{
// 			printf("%s:%i //", arg->content, arg->to_replace);
// 			arg = arg->next;
// 		}
// 		printf(")");
// 		first = first->next;
// 	}
// }
