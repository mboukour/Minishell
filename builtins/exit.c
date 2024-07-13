/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:53:09 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 19:08:02 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exit_core(int status)
{
	rl_clear_history();
	smart_close();
	free_program();
	smart_free();
	exit(status);
	return (0);
}

bool	is_num(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (str[i] > '9' || str[i] < '0')
			return (false);
		i++;
	}
	return (true);
}

int	count_sp(char *str)
{
	int	i;

	i = 0;
	while (str[i] && is_ws(str[i]))
		i++;
	return (i);
}

int	exit_cmd(t_treenode *root)
{
	t_arg	*args;
	bool	status;
	int		exit_status;

	args = root->args;
	status = false;
	ft_putstr_fd(1, "exit\n");
	if (!args)
		exit_core(0);
	exit_status = ft_atoi(args->content + count_sp(args->content), &status);
	if (!is_num(args->content + count_sp(args->content)) || status)
	{
		ft_putstr_fd(2, "exit: ");
		ft_putstr_fd(2, args->content);
		ft_putstr_fd(2, ": numeric argument required\n");
		exit_core(NUMERIC_ARG_STATUS);
		return (1);
	}
	else if (args->next)
	{
		ft_putstr_fd(2, "exit: too many arguments\n");
		return (1);
	}
	exit_core(exit_status);
	return (0);
}
