/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specefic_expanders.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:58:10 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/14 23:52:03 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_management.h"

static void	handle_look_for(t_exp_vars *vars, t_arg *arg, int after_star)
{
	vars->look_for = arg->content;
	vars->append_after = NULL;
	if (after_star != NONE)
	{
		vars->look_for = ft_substr(arg->content, 0, after_star, GC);
		vars->append_after = ft_substr(arg->content, after_star,
				ft_strlen(arg->content), GC);
	}
}

int	general_expand(t_exp_vars *vars, t_arg **arg, int after_star)
{
	handle_look_for(vars, *arg, after_star);
	vars->env_node = strict_get_env(vars->env, vars->look_for + 1);
	if (vars->env_node && vars->append_after)
	{
		vars->env_node = ft_lstnew_env(vars->env_node->value, GC);
		vars->env_node->value = ft_strjoin(vars->env_node->value,
				vars->append_after, GC);
		vars->env_node->star_to_replace = (*arg)->to_replace;
		return (0);
	}
	else if (!vars->env_node && vars->append_after)
	{
		(*arg)->content = vars->append_after;
		return (2);
	}
	else if (!vars->env_node || !vars->env_node->envyable)
	{
		remove_arg_node(&vars->looping_cmd->arg, *arg);
		*arg = vars->move;
		return (3);
	}
	return (0);
}

int	specefic_expand(t_exp_vars *vars, t_arg *arg)
{
	vars->prev = arg->prev;
	vars->next = arg->next;
	vars->expanded_env = env_to_cmd_arg(vars->env_node);
	vars->last_expanded = ft_lstlast_cmd_arg(vars->expanded_env);
	if (!vars->env_node->after_joinable && !vars->env_node->before_joinable
		&& handle_both_not_joinable(vars))
		return (1);
	else if (vars->env_node->before_joinable && vars->env_node->after_joinable
		&& handle_both_joinable(vars))
	{
		if (vars->go)
			return (1);
		return (0);
	}
	else if (vars->env_node->after_joinable && handle_only_after_joinable(vars))
	{
		if (vars->go)
			return (1);
		return (0);
	}
	else if (vars->env_node->before_joinable
		&& handle_only_before_joinable(vars))
		return (1);
	return (0);
}
