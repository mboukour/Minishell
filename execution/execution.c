/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabrouk <amabrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 20:05:08 by amabrouk          #+#    #+#             */
/*   Updated: 2024/06/06 16:24:31 by amabrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"



void	pipeline(t_treenode *root, t_data *data, t_env **env)
{
	pid_t	pid1;
	pid_t	pid2;
	char	*exp;

	if (pipe(data->end) == -1)
	{
		perror("PIPE ERROR");
		exit(EXIT_FAILURE);
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("FORK FAILED");
		exit(EXIT_FAILURE);
	}
	else if (pid1 == 0)
	{
		close(data->end[0]);
		if (dup2(data->end[1], 1) == -1)
		{
			perror("DUP ERROR IN CHILD");
			exit(EXIT_FAILURE);
		}
		close(data->end[1]);

		traverse_tree(root->left, data, env);
		exit(EXIT_FAILURE);
	}
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("FORK FAILED");
		exit(EXIT_FAILURE);
	}
	else if (pid2 == 0)
	{
		close(data->end[1]);
		if (dup2(data->end[0], 0) == -1)
		{
			perror("DUP ERROR IN PARENT");
			exit(EXIT_FAILURE);
		}
		close(data->end[0]);
		traverse_tree(root->right, data, env);
		exit(EXIT_FAILURE);
	}
	close(data->end[0]);
	close(data->end[1]);
	while (wait(NULL) != -1)
		;
}

// SET BUILTINS EXIT STATUS!!!!
void execute_builtin(t_treenode *root, t_env **envp, t_data *data)
{
	int builtin;

	builtin = root->builtin;
	if(builtin == ENV_CMD)
		data->status = env(*envp);
	else if(builtin == ECHO)
		data->status = echo(root);
	else if(builtin == PWD)
		data->status = pwd(root);
	else if(builtin == EXPORT)
		data->status = export(envp, root);
	else if(builtin == UNSET)
		data->status = unset(envp, root);
	else if(builtin == CD)
		data->status = cd(root, envp);
	else if(builtin == EXIT)
		data->status = exit_cmd(root, *envp);
}


int change_status(t_env **env, int new_status)
{
	char *current_status;
	char *final;

	current_status = ft_itoa(new_status, MANUAL);
	if(!current_status)
		return (1);
	final = ft_strjoin("?=", current_status, MANUAL);
	if(!final)
		return (free(current_status), 1);
	free(current_status);
	export_core(env, final);
	return (0);
}

char *get_underscore(t_treenode *root)
{
	t_arg *args;
	char *result;

	args = root->args;
	while(args)
	{
		result = args->content;
		args = args->next;
	}
	return (result);
}

void execute_command(t_treenode *root, t_env **env, t_data *data)
{
	pid_t pid;
	char *exp;
	char *absolute_path;

	exp = ft_strjoin("_=", get_underscore(root) , MANUAL);
	export_core(env, exp);
	free(exp);
	if(root->builtin != NONE)
	{
		execute_builtin(root, env, data);
		return;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("FORK FAILED");
		return ;
	}
	else if (pid == 0)
	{
		get_path(root, *env, data);
		data->env = env_to_array(*env);
		data->cmd = args_to_arr(root->args);
		if (execve(data->path, data->cmd, data->env) == -1)
		{
			write(2, root->content, ft_strlen(root->content));
			write(2, ": command not found\n", 20);
			ft_lstclear_env(*env);
			smart_free();
			exit(127);
		}
	}
	else
	{
		waitpid(pid, &data->status, 0);
		if (WIFSIGNALED(data->status))
		{
			data->status += 128;
			change_status(env, data->status);
			return ;
		}
		WIFEXITED(data->status);
		data->status = WEXITSTATUS(data->status);
		change_status(env, data->status);
	}
}

void	handle_red(t_redir *redir)
{
	int	fd;

	fd = -2;
	while (redir)
	{
		if (redir->token == DREDIR_OUT)
			fd = open(redir->redir_string, O_RDWR | O_CREAT | O_APPEND, 0777);
		else if (redir->token == REDIR_OUT)
			fd = open(redir->redir_string, O_RDWR | O_CREAT | O_TRUNC, 0777);
		if (fd == -1)
		{
			perror("File Descriptor");
			exit(EXIT_FAILURE);
		}
		if (dup2(fd, 1) == -1)
		{
			perror("handle red dup failed");
			close(fd);
			exit(EXIT_FAILURE);
		}
		close(fd);
		redir = redir->next;
	}
}
