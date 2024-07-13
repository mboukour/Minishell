/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keep_track.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:03:06 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 04:11:22 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*globalizer_manual(int mode, void *ptr)
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

void	null_protector(void *to_check)
{
	if (!to_check)
		error_procedure();
}

void	store_malloced(void *ptr_to_add)
{
	t_node	*head;
	t_node	*new_node;

	null_protector(ptr_to_add);
	head = globalizer_manual(GET, NULL);
	new_node = ft_lstnew(ptr_to_add);
	null_protector(new_node);
	if (!head)
		globalizer_manual(SET, new_node);
	else
		ft_lstadd_back(head, new_node);
}

void	free_program(void)
{
	t_node	*head;

	head = globalizer_manual(GET, NULL);
	ft_lstclear(&head);
	globalizer_manual(SET, NULL);
}

int	remove_ptr(void *ptr_to_rm)
{
	t_node	*head;

	head = globalizer_manual(GET, NULL);
	if (!head)
		return (0);
	while (head)
	{
		if (head->ptr_to_free == ptr_to_rm)
		{
			if (head->prev)
				head->prev->next = head->next;
			else
			{
				if (head->next)
					head->next->prev = NULL;
				globalizer_manual(SET, head->next);
			}
			if (head->next)
				head->next->prev = head->prev;
			free(head);
			return (1);
		}
		head = head->next;
	}
	return (0);
}
