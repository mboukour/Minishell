/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinables_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 00:23:48 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/15 01:49:29 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../env_management.h"

void	only_joinable_after_prev(t_exp_vars *vars)
{
	t_cmd_arg	*set_cmd;

	vars->prev->next = NULL;
	vars->prev = ft_lstfirst_arg(vars->prev);
	set_cmd = ft_lstnew_cmd_arg(vars->prev);
	insert_before_cmd_arg(vars->cmd_arg, vars->looping_cmd, set_cmd);
}

void	only_joinable_after_next(t_exp_vars *vars)
{
	t_cmd_arg	*set_cmd;

	if (!is_nextable(vars->next, vars->env))
	{
		vars->next->prev = NULL;
		set_cmd = ft_lstnew_cmd_arg(vars->next);
		insert_after_cmd_arg(vars->last_expanded, set_cmd);
		vars->next_lp_cmd = set_cmd;
	}
	else
	{
		ft_lstlast_arg(vars->last_expanded->arg)->next = vars->next;
		if (is_env(vars->next->content))
		{
			vars->next->prev = vars->last_expanded->arg;
			vars->move = vars->next;
			vars->go = false;
			vars->looping_cmd = vars->last_expanded;
		}
		else
			vars->next->prev = NULL;
	}
}

void	both_joinable_prev(t_exp_vars *vars)
{
	vars->prev->next = NULL;
	vars->prev->next = vars->expanded_env->arg;
	vars->expanded_env->arg->prev = vars->prev;
	vars->expanded_env->arg = vars->prev;
}

void	both_joinable_next(t_exp_vars *vars)
{
	t_cmd_arg	*set_cmd;

	if (!is_nextable(vars->next, vars->env))
	{
		vars->next->prev = NULL;
		set_cmd = ft_lstnew_cmd_arg(vars->next);
		insert_after_cmd_arg(vars->last_expanded, set_cmd);
		vars->next_lp_cmd = set_cmd;
	}
	else
	{
		ft_lstlast_arg(vars->last_expanded->arg)->next = vars->next;
		if (is_env(vars->next->content))
		{
			vars->next->prev = vars->last_expanded->arg;
			vars->move = vars->next;
			vars->go = false;
			vars->looping_cmd = vars->last_expanded;
		}
		else
			vars->next->prev = NULL;
	}
}
