/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:58:10 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 21:15:38 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_management.h"


void	handle_both_not_joinable(t_exp_vars *vars)
{
	t_cmd_arg	*set_cmd;
	t_arg		*prev;
	t_arg		*next;

	fprintf(stderr, "I WORLK\n");
	set_cmd = NULL;
	if(vars->prev)
	{
		vars->prev->prev->next = NULL;
		vars->prev = ft_lstfirst_arg(prev);
		set_cmd = ft_lstnew_cmd_arg(prev);
		insert_before_cmd_arg(vars->cmd_arg, vars->looping_cmd, set_cmd);
	}
	replace_cmd_arg_node(vars->cmd_arg, vars->looping_cmd, vars->expanded_env);
	if(vars->next)
	{
		next->prev = NULL;
		set_cmd = ft_lstnew_cmd_arg(next);
		insert_after_cmd_arg(vars->last_expanded, set_cmd);
		vars->next_lp_cmd = set_cmd;
	}
}

// bool the_expander(t_exp_vars *vars)
// {

// 	if(!env_node->after_joinable && !env_node->before_joinable)
// 	{
		
// 	}
// 	return (true);
// }