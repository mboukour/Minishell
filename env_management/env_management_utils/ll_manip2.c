/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ll_manip2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 18:34:53 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 18:35:20 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../env_management.h"


void insert_before_cmd_arg(t_cmd_arg **head, t_cmd_arg *node, t_cmd_arg *new_node)
{
	if (!node) 
		return;

	new_node->next = node;
	new_node->prev = node->prev;
	if (node->prev)
		node->prev->next = new_node;
	else 
		*head = new_node;
	node->prev = new_node;
}


void insert_after_cmd_arg(t_cmd_arg *node, t_cmd_arg *new_node)
{
	if (!node) 
		return;
	new_node->prev = node;
	new_node->next = node->next;
	if (node->next)
		node->next->prev = new_node;
	node->next = new_node;
}
