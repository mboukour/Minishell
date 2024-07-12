/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:08:07 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/12 17:08:16 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgc.h"

void	*globalizer_head(int mode, void *ptr)
{
	static t_node	*head;

	if (mode == SET)
	{
		head = ptr;
		return (NULL);
	}
	else
		return (head);
}

int	store_mallocs(void *ptr_to_add)
{
	t_node	*head;
	t_node	*new_node;

	if (!ptr_to_add)
		return (ERROR);
	head = globalizer_head(GET, NULL);
	new_node = ft_lstnew(ptr_to_add);
	if (!new_node)
		error_procedure();
	if (!head)
	{
		globalizer_head(SET, new_node);
		return (SUCCESS);
	}
	else
	{
		ft_lstadd_back(head, new_node);
		return (SUCCESS);
	}
}

void	*smart_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (ptr && store_mallocs(ptr))
		return (ptr);
	error_procedure();
	return (NULL);
}

void	smart_free(void)
{
	t_node	*head;

	head = globalizer_head(GET, NULL);
	ft_lstclear(&head);
	globalizer_head(SET, NULL);
}
