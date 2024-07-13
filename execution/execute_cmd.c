/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 17:43:12 by amabrouk          #+#    #+#             */
/*   Updated: 2024/07/13 21:17:53 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*get_underscore(t_treenode *root)
{
	t_arg	*args;
	char	*result;

	args = root->args;
	result = NULL;
	while (args)
	{
		result = args->content;
		args = args->next;
	}
	return (result);
}

int	execute_builtin(t_treenode *root, t_env **envp, t_data *data)
{
	char	*command;

	command = root->content;
	if (!ft_strcmp(command, "env"))
		data->status = env(*envp);
	else if (!ft_strcmp(command, "echo"))
		data->status = echo(root);
	else if (!ft_strcmp(command, "pwd"))
		data->status = pwd(data);
	else if (!ft_strcmp(command, "export"))
		data->status = export(envp, root);
	else if (!ft_strcmp(command, "unset"))
		data->status = unset(envp, root);
	else if (!ft_strcmp(command, "cd"))
		data->status = cd(root, envp, data);
	else if (!ft_strcmp(command, "exit"))
		data->status = exit_cmd(root, data);
	else
		return (NONE);
	change_status(envp, data->status);
	return (0);
}

void	execute_cmd_p3(t_treenode *root, t_env **env, t_data *data)
{
	t_arg	*args;

	args = NULL;
	data->env = env_to_array(*env);
	args = ft_lstnew_arg(NULL);
	args->content = ft_strdup(root->content, GC);
	args->next = root->args;
	data->cmd = args_to_arr(args);
	signal(SIGQUIT, SIG_DFL);
	execve(root->content, data->cmd, data->env);
	get_path(root, *env, data);
	if (!access(data->path, F_OK) && access(data->path, X_OK) == -1)
	{
		ft_putstr_fd(2, root->content);
		ft_putstr_fd(2, ": ");
		perror(NULL);
		exit(PERMISSION_STATUS);
	}
	execve(data->path, data->cmd, data->env);
	write(2, root->content, ft_strlen(root->content));
	write(2, ": command not found\n", 20);
	smart_free();
	free_program();
	exit(CMD_NOT_FOUND);
}

void	execute_cmd_p2(t_treenode *root, t_env **env, t_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		pipeline_error("fork: ", FORK_ERROR_STATUS);
		return ;
	}
	else if (pid == 0)
		execute_cmd_p3(root, env, data);
	else
	{
		waitpid(pid, &data->status, 0);
		if (WIFSIGNALED(data->status))
		{
			data->status += 128;
			change_status(env, data->status);
			return ;
		}
		data->status = WEXITSTATUS(data->status);
		change_status(env, data->status);
	}
}

void	execute_command(t_treenode *root, t_env **env, t_data *data)
{
	char	*exp;
	char	*under;

	if (!root->content)
		return ;
	under = get_underscore(root);
	if (!under)
		under = root->content;
	exp = ft_strjoin("_=", under, MANUAL);
	null_protector(exp);
	export_core(env, exp);
	free(exp);
	if (!execute_builtin(root, env, data))
		return ;
	if (root->is_a_directory)
	{
		ft_putstr_fd(2, root->content);
		ft_putstr_fd(2, ": Is a directory\n");
		change_status(env, DIRECORY_STATUS);
		return ;
	}
	execute_cmd_p2(root, env, data);
}
