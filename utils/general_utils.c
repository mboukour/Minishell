/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 05:20:14 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/18 18:48:02 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_t_data(t_data *data)
{
	data->infile = -1;
	data->outfile = -1;
	data->end[0] = -1;
	data->end[1] = -1;
	data->status = 0;
	data->cmd = NULL;
	data->path = NULL;
	data->env = NULL;
	data->show_oldpwd = true;
	data->show_pwd = true;
	data->pwd = getcwd(NULL, 0);
	if (!data->pwd)
	{
		ft_putstr_fd(2, "error retrieving current directory: ");
		ft_putstr_fd(2, "getcwd: cannot access parent directories: ");
		perror("");
	}
	else
		store_malloced(data->pwd);
}

bool	is_all_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!is_ws(str[i]))
			return (false);
		i++;
	}
	return (true);
}

void	sigint_handler(int sig)
{
	t_env	**env;

	(void)sig;
	env = globalizer_env(GET, NULL);
	exp_w_null(env, "?=1");
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	save_terminal(struct termios *saved_attributes, t_env **env)
{
	if (tcgetattr(STDIN_FILENO, saved_attributes) != 0)
	{
		perror("Failed to get terminal attributes");
		exp_w_null(env, "?=1");
	}
}

void	restore_terminal(const struct termios *saved_attributes, t_env **env)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, saved_attributes) != 0)
	{
		perror("Failed to set terminal attributes");
		exp_w_null(env, "?=1");
	}
}
