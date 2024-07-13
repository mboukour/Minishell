/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_cmd_arg.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 04:12:49 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 04:12:51 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd_arg	*ft_lstnew_cmd_arg(t_arg *arg)
{
	t_cmd_arg	*new_node;

	new_node = smart_malloc(sizeof(t_cmd_arg));
	new_node->arg = arg;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

t_cmd_arg	*ft_lstlast_cmd_arg(t_cmd_arg *lst)
{
	t_cmd_arg	*temp;

	temp = lst;
	while (temp)
	{
		if (!temp->next)
			return (temp);
		temp = temp->next;
	}
	return (lst);
}

void	ft_lstaddback_cmd_arg(t_cmd_arg **lst, t_cmd_arg *new)
{
	t_cmd_arg	*last_cmd_arg;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last_cmd_arg = ft_lstlast_cmd_arg(*lst);
	last_cmd_arg->next = new;
	new->prev = last_cmd_arg;
}

// void ft_lstiter_cmd_arg(t_cmd_arg *cmd_arg)
// {
// 	t_arg *arg;

// 	printf("---------------------\n");
// 	while(cmd_arg)
// 	{
// 		arg = cmd_arg->arg;
// 		while(arg)
// 		{
// 			printf("%s //", arg->content);
// 			arg = arg->next;
// 		}
// 		cmd_arg = cmd_arg->next;
// 		printf("\n");
// 	}
// 	printf("---------------------\n");
// }