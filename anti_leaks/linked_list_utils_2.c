/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_utils_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:21:42 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/12 17:21:47 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgc.h"

t_fd	*ft_lstnew_fd(int fd_to_store)
{
	t_fd	*new_node;

	new_node = smart_malloc(sizeof(t_fd));
	new_node->fd = fd_to_store;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

t_fd	*ft_lstlast_fd(t_fd *lst)
{
	t_fd	*temp;

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

void	ft_lstadd_back_fd(t_fd *lst, t_fd *new_node)
{
	t_fd	*last;

	if (!lst)
		return ;
	last = ft_lstlast_fd(lst);
	last->next = new_node;
	new_node->prev = last;
}
