/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinables_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 00:07:27 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/14 00:28:51 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../env_management.h"

int	handle_both_not_joinable(t_exp_vars *vars)
{
	t_cmd_arg	*set_cmd;

	set_cmd = NULL;
	if (vars->prev)
	{
		vars->prev->next = NULL;
		vars->prev = ft_lstfirst_arg(vars->prev);
		set_cmd = ft_lstnew_cmd_arg(vars->prev);
		insert_before_cmd_arg(vars->cmd_arg, vars->looping_cmd, set_cmd);
	}
	replace_cmd_arg_node(vars->cmd_arg, vars->looping_cmd, vars->expanded_env);
	if (vars->next)
	{
		vars->next->prev = NULL;
		set_cmd = ft_lstnew_cmd_arg(vars->next);
		insert_after_cmd_arg(vars->last_expanded, set_cmd);
		vars->next_lp_cmd = set_cmd;
	}
	return (1);
}

int	handle_both_joinable(t_exp_vars *vars)
{
	if (vars->prev)
		both_joinable_prev(vars);
	replace_cmd_arg_node(vars->cmd_arg, vars->looping_cmd, vars->expanded_env);
	if (vars->next)
		both_joinable_next(vars);
	return (1);
}

int	handle_only_after_joinable(t_exp_vars *vars)
{
	t_cmd_arg	*set_cmd;

	set_cmd = NULL;
	if (vars->prev)
		only_joinable_after_prev(vars);
	replace_cmd_arg_node(vars->cmd_arg, vars->looping_cmd, vars->expanded_env);
	if (vars->next)
		only_joinable_after_next(vars);
	return (1);
}

int	handle_only_before_joinable(t_exp_vars *vars)
{
	t_cmd_arg	*set_cmd;

	set_cmd = NULL;
	if (vars->prev)
	{
		vars->prev->next = NULL;
		vars->prev = ft_lstfirst_arg(vars->prev);
		vars->prev->next = vars->expanded_env->arg;
		vars->expanded_env->arg = vars->prev;
	}
	if (vars->next)
	{
		vars->next->prev = NULL;
		set_cmd = ft_lstnew_cmd_arg(vars->next);
		insert_after_cmd_arg(vars->last_expanded, set_cmd);
		vars->next_lp_cmd = set_cmd;
	}
	replace_cmd_arg_node(vars->cmd_arg, vars->looping_cmd, vars->expanded_env);
	return (1);
}
