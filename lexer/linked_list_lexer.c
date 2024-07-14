/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_lexer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 01:58:32 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/14 02:11:39 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_lex	*ft_lstnew_lex(char *content, int token, int len, int join_count)
{
	t_lex	*new_node;

	new_node = smart_malloc(sizeof(t_lex));
	new_node->content = content;
	new_node->len = len;
	new_node->token = token;
	new_node->to_replace = REPLACE_ALL;
	new_node->join_count = join_count;
	new_node->here_doc_fd = NONE;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

t_lex	*ft_lstlast_lex(t_lex *lst)
{
	t_lex	*temp;

	if (lst == NULL)
		return (NULL);
	temp = lst;
	while (temp)
	{
		if (!temp->next)
			return (temp);
		temp = temp->next;
	}
	return (lst);
}

void	ft_lstadd_back_lex(t_lex **lst, t_lex *new)
{
	t_lex	*last_lex;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last_lex = ft_lstlast_lex(*lst);
	last_lex->next = new;
	new->prev = last_lex;
}
