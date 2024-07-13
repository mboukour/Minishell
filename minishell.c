/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 15:53:09 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 16:18:48 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_error(int error_checker, t_lex *lex, t_env **env);
void	init_t_data(t_data *data);
bool	is_all_space(char *str);
void	sigint_handler(int sig);
void	save_terminal(struct termios *saved_attributes);
void	restore_terminal(const struct termios *saved_attributes);
int		traverse_tree(t_treenode *root, t_data *data, t_env **env);

bool	handle_lex_error(t_lex *lexed, t_env **env)
{
	t_lex	*we_check_lex;
	int		in1;

	we_check_lex = lex_input_checker(lexed);
	if (we_check_lex)
	{
		in1 = dup(STDIN_FILENO);
		if (in1 == -1)
			return (perror("dup:"), export_core(env, "?=1"), NULL);
		fake_open(lexed, we_check_lex);
		if (dup2(in1, STDIN_FILENO) == -1 || g_heredoc_sigint)
			export_core(env, "?=1");
		g_heredoc_sigint = 0;
		display_error(NONE, we_check_lex, env);
		return (false);
	}
	return (true);
}

t_treenode	*parsing(char *input, t_env **env)
{
	t_lex		*lexed;
	t_middle	*middled;
	int			error_checker;
	int			in1;

	lexed = tokenizer(input);
	error_checker = open_checker(lexed);
	if (error_checker != NONE)
		return (display_error(error_checker, NULL, env), NULL);
	quotes_handler(lexed);
	if (handle_lex_error(lexed, env))
	{
		middled = make_middle(lexed);
		in1 = dup(STDIN_FILENO);
		if (in1 == -1)
			return (perror("dup:"), export_core(env, "?=1"), NULL);
		valid_here_doc(middled);
		if (dup2(in1, STDIN_FILENO) == -1 || g_heredoc_sigint)
			return (export_core(env, "?=1"), NULL);
		g_heredoc_sigint = 0;
		return (ruined_tree(middled));
	}
	return (NULL);
}

int	launch_minishell(t_env **env, t_data *data)
{
	char			*input;
	t_treenode		*root;
	struct termios	saved_attributes;

	while (true)
	{
		save_terminal(&saved_attributes);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, sigint_handler);
		input = readline("\x1b[34m🐐 GoatShell\x1b[0m ");
		store_mallocs(input);
		if (!input)
			return (ft_putstr_fd(1, "exit\n"), exit_core(0), 0);
		else if (!is_all_space(input) && ft_strcmp(input, ""))
		{
			root = parsing(input, env);
			if (root)
				traverse_tree(root, data, env);
			g_heredoc_sigint = 0;
		}
		add_history(input);
		restore_terminal(&saved_attributes);
	}
	return (0);
}

int	no_envp_export(t_env **env, t_data *data)
{
	char	*pwd;

	if (data->pwd)
	{
		pwd = ft_strjoin("PWD=", data->pwd, MANUAL);
		null_protector(pwd);
		export_core(env, pwd);
		free(pwd);
	}
	export_core(env, "SHLVL=1");
	export_core(env, SECURE_PATH);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env;
	t_data	data;

	(void)ac;
	(void)av;
	if (!isatty(0))
		return (0);
	rl_catch_signals = 0;
	init_t_data(&data);
	if (!envp || !*envp)
	{
		env = NULL;
		if (no_envp_export(&env, &data))
			return (1);
	}
	else
	{
		env = array_to_env(envp);
		if (!env)
			return (ft_putstr_fd(2, FAILURE_MSG), 1);
	}
	export_core(&env, "?=0");
	launch_minishell(&env, &data);
	return (0);
}
