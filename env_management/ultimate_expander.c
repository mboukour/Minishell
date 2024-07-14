/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ultimate_expander.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 00:13:16 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/14 01:03:13 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_management.h"

int	process_real_env(t_arg **arg, t_exp_vars *vars, int after_star)
{
	int	res;

	if ((*arg)->content[1] && is_c_num((*arg)->content[1]))
	{
		(*arg)->content += 2;
		*arg = vars->move;
		return (3);
	}
	res = general_expand(vars, arg, after_star);
	if (res)
		return (res);
	else if (specefic_expand(vars, *arg))
		return (2);
	return (0);
}

void	process_arg(t_arg *arg, t_exp_vars *vars)
{
	int	after_star;
	int	res;

	while (arg)
	{
		vars->go = true;
		vars->move = (arg)->next;
		after_star = after_env_star((arg)->content);
		if ((arg)->to_replace == REPLACE_ALL && ((arg)->token == ENV
				|| is_env((arg)->content) || after_star != NONE))
		{
			res = process_real_env(&arg, vars, after_star);
			if (res == 2)
				break ;
			else if (res == 3)
				continue ;
		}
		else if ((arg)->to_replace == ONLY_ENV)
		{
			fprintf(stderr, "ONLY ENV: {%s}\n", arg->content);
			expand_only_env(&vars->looping_cmd->arg, vars->env, arg);
		}
		arg = vars->move;
	}
}

void	ultimate_expander(t_cmd_arg **cmd_arg, t_env *env)
{
	t_exp_vars	vars;

	vars.looping_cmd = *cmd_arg;
	vars.cmd_arg = cmd_arg;
	vars.env = env;
	while (vars.looping_cmd)
	{
		vars.next_lp_cmd = vars.looping_cmd->next;
		process_arg(vars.looping_cmd->arg, &vars);
		vars.looping_cmd = vars.next_lp_cmd;
	}
}
