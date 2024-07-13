/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ll_manip.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 18:33:08 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 18:42:58 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../env_management.h"


void remove_arg_node(t_arg **head_ref, t_arg *node_to_remove)
{
	if (*head_ref == NULL || node_to_remove == NULL)
		return;
	if (*head_ref == node_to_remove)
		*head_ref = node_to_remove->next;
	if (node_to_remove->next != NULL)
		node_to_remove->next->prev = node_to_remove->prev;
	if (node_to_remove->prev != NULL)
		node_to_remove->prev->next = node_to_remove->next;
	node_to_remove->next = NULL;
	node_to_remove->prev = NULL;
}
void replace_arg_with_list(t_arg **head_ref, t_arg *node_to_replace, t_arg *new_list_head)
{
	t_arg *new_list_tail;

	if (*head_ref == NULL || node_to_replace == NULL || new_list_head == NULL)
		return;
	if (*head_ref == node_to_replace)
		*head_ref = new_list_head;
	if (node_to_replace->prev != NULL)
	{
		node_to_replace->prev->next = new_list_head;
		new_list_head->prev = node_to_replace->prev;
	}
	else
		new_list_head->prev = NULL;
	new_list_tail = ft_lstlast_arg(new_list_head);
	if (node_to_replace->next != NULL)
	{
		node_to_replace->next->prev = new_list_tail;
		new_list_tail->next = node_to_replace->next;
	}
	else
		new_list_tail->next = NULL;
}

void replace_cmd_arg_node(t_cmd_arg **head, t_cmd_arg *node, t_cmd_arg *new_head)
{
	t_cmd_arg *new_tail;
	if (node == NULL || new_head == NULL || head == NULL || *head == NULL)
	{
			return;
	}
	if (node->prev != NULL) {
		node->prev->next = new_head;
		new_head->prev = node->prev;
	} else {
		*head = new_head;
		new_head->prev = NULL;
	}
	new_tail = ft_lstlast_cmd_arg(new_head);
	if (node->next != NULL) 
	{
		node->next->prev = new_tail;
		new_tail->next = node->next;
	}
	else 
		new_tail->next = NULL;
}


void remove_cmd_arg_node(t_cmd_arg **head, t_cmd_arg *node)
{
	if (node == NULL || head == NULL || *head == NULL)
		return;
	if (node->prev != NULL)
		node->prev->next = node->next;
	else
		*head = node->next;
	if (node->next != NULL)
		node->next->prev = node->prev;
}
