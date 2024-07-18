/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_expanders.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 01:32:55 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/18 20:44:26 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_management.h"

static void	append_final_arg(t_arg **head, char *ready_arg, int to_replace)
{
	t_arg	*current;

	current = ft_lstnew_arg(NULL);
	current->content = ready_arg;
	current->to_replace = to_replace;
	ft_lstaddback_arg(head, current);
}

static void	handle_broken_env(t_exp_vars *vars, t_lex **hold,
	t_lex *broken, int after_star)
{
	vars->append_after = NULL;
	vars->look_for = broken->content;
	after_star = after_env_star(broken->content);
	if (after_star != NONE)
	{
		vars->look_for = ft_substr(broken->content, 0, after_star, GC);
		vars->append_after = ft_substr(broken->content, after_star,
				ft_strlen(broken->content), GC);
	}
	vars->go = true;
	vars->env_node = strict_get_env(vars->env, vars->look_for + 1);
	if (vars->env_node && vars->env_node->envyable)
		broken->content = ft_strjoin(get_real_env(vars->env_node->value),
				vars->append_after, GC);
	else
		remove_lex_node(hold, broken);
}

t_arg	*final_args(t_cmd_arg *cmd_arg)
{
	t_arg	*args;
	t_arg	*head;
	char	*ready_arg;
	int		to_replace;

	head = NULL;
	ready_arg = NULL;
	to_replace = REPLACE_ALL;
	while (cmd_arg)
	{
		args = cmd_arg->arg;
		while (args)
		{
			if (am_i_a_star(args->content) && args->to_replace < to_replace)
				to_replace = args->to_replace;
			ready_arg = ft_strjoin(ready_arg, args->content, GC);
			args = args->next;
		}
		if (ready_arg)
			append_final_arg(&head, ready_arg, to_replace);
		cmd_arg = cmd_arg->next;
		ready_arg = NULL;
	}
	return (head);
}

void	expand_only_env(t_arg **arg_head, t_env *env, t_arg *arg)
{
	t_lex		*broken;
	t_lex		*hold;
	int			after_star;
	t_exp_vars	vars;

	vars.go = false;
	vars.env = env;
	vars.arg = arg;
	if (vars.arg->to_replace == ONLY_ENV)
	{
		broken = tokenizer(vars.arg->content);
		hold = broken;
		while (broken)
		{
			vars.broken_next = broken->next;
			if (broken->token == ENV)
				handle_broken_env(&vars, &hold, broken, after_star);
			broken = vars.broken_next;
		}
		if (!vars.go)
			return ;
		vars.arg->content = lex_to_str(hold);
		if (!vars.arg->content)
			vars.arg->content = ft_strdup("", GC);
	}
}


int	handle_arg_star(t_exp_vars *vars, t_cmd_arg **cmd_arg, t_arg *arg)
{
	t_cmd_arg	*expanded_star;
	t_cmd_arg	*set_cmd;

	vars->next = arg->next;
	vars->prev = arg->prev;
	expanded_star = cmd_arg_star_matching(arg->content);
	if (!expanded_star)
		return (0);
	if (vars->prev)
	{
		vars->prev->next = NULL;
		vars->prev = ft_lstfirst_arg(vars->prev);
		set_cmd = ft_lstnew_cmd_arg(vars->prev);
		insert_before_cmd_arg(cmd_arg, vars->looping_cmd, set_cmd);
	}
	replace_cmd_arg_node(cmd_arg, vars->looping_cmd, expanded_star);
	if (vars->next)
	{
		vars->next->prev = NULL;
		set_cmd = ft_lstnew_cmd_arg(vars->next);
		insert_after_cmd_arg(expanded_star, set_cmd);
		vars->next_lp_cmd = set_cmd;
		return (1);
	}
	return (0);
}
