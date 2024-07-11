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


int change_status(t_env **env, int new_status)
{
	char *current_status;
	char *final;

	current_status = ft_itoa(new_status, MANUAL);
	null_protector(current_status);
	final = ft_strjoin("?=", current_status, MANUAL);
	null_protector(final);
	free(current_status);
	export_core(env, final);
	free(final);
	return (0);
}

void pipeline_error(char *first, int status)
{
	perror(first);
	exit_core(status);
}

void	pipeline(t_treenode *root, t_data *data, t_env **env)
{
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(data->end) == -1)
	{
		perror("pipe: ");
		init_tree(root);
		change_status(env, EXIT_FAILURE);
		return;
	}
	pid1 = fork();
	if (pid1 == -1)
		pipeline_error("fork: ", FORK_ERROR_STATUS);
	else if (pid1 == 0)
	{
		close(data->end[0]);
		if (dup2(data->end[1], 1) == -1)
		{
			close(data->end[1]);
			pipeline_error("dup2: ", EXIT_FAILURE);
		}
		close(data->end[1]);
		traverse_tree(root->left, data, env);
		exit(data->status);
	}
	pid2 = fork();
	if (pid2 == -1)
		pipeline_error("fork: ", FORK_ERROR_STATUS);
	else if (pid2 == 0)
	{
		close(data->end[1]);
		if (dup2(data->end[0], 0) == -1)
		{
			close(data->end[0]);
			pipeline_error("dup2: ", EXIT_FAILURE);
		}
		close(data->end[0]);
		traverse_tree(root->right, data, env);
		exit(data->status);
	}
	close(data->end[0]);
	close(data->end[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &data->status, 0);
	data->status = WEXITSTATUS(data->status);
	change_status(env, data->status);
}

int execute_builtin(t_treenode *root, t_env **envp, t_data *data)
{
	char *command;

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
		data->status = exit_cmd(root);
	else
		return (NONE);
	change_status(envp, data->status);
	return (0);
}

char *get_underscore(t_treenode *root)
{
	t_arg *args;
	char *result;

	args = root->args;
	result = NULL;
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
	t_arg *args;
	char *under;

	if(!root->content || infooo == -1)
		return;
	under = get_underscore(root);
	if(!under)
		under = root->content;
	exp = ft_strjoin("_=",  under, MANUAL);
	null_protector(exp);
	export_core(env, exp);
	free(exp);
	if (!execute_builtin(root, env, data))
		return;
	if(root->is_a_directory)
	{
		ft_putstr_fd(2, root->content);
		ft_putstr_fd(2, ": Is a directory\n");
		change_status(env, DIRECORY_STATUS);
		return;
	}
	pid = fork();
	if (pid == -1)
	{
		pipeline_error("fork: ", FORK_ERROR_STATUS);
		return ;
	}
	else if (pid == 0)
	{
		data->env = env_to_array(*env);
		args = ft_lstnew_arg(NULL);
		args->content = ft_strdup(root->content, GC);
		args->next = root->args;
		data->cmd = args_to_arr(args);
		execve(root->content, data->cmd, data->env);
		get_path(root, *env, data);
		if (!access(data->path, F_OK) && access(data->path, X_OK) == -1)
        {
            ft_putstr_fd(2, root->content);
            ft_putstr_fd(2, ": ");
            perror(NULL);
            exit(PERMISSION_STATUS);
        }
		if (execve(data->path, data->cmd, data->env) == -1)
		{
			write(2, root->content, ft_strlen(root->content));
			write(2, ": command not found\n", 20);
			smart_free();
			free_program();
			exit(CMD_NOT_FOUND);
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
		data->status = WEXITSTATUS(data->status);
		change_status(env, data->status);
	}
}

void	init_tree(t_treenode *root)
{
	root->after_redir = NULL;
	root->args = NULL;
	root->before_redir = NULL;
	root->builtin = -1;
	root->content = NULL;
	root->left = NULL;
	root->right = NULL;
	root->to_replace = -1;
}



void remove_lex_node(t_lex **head, t_lex *node) 
{
    if (node == NULL || head == NULL || *head == NULL) 
        return;
    if (node == *head)
        *head = node->next;
    if (node->prev != NULL)
        node->prev->next = node->next;
    if (node->next != NULL) 
        node->next->prev = node->prev;
}


void expand_mini_lexed(t_lex **mini, t_env *env)
{
	t_env *env_node;
	t_lex *looping_node;
	t_lex *next;

	looping_node = *mini;
	while (looping_node)
	{
		next = looping_node->next;
		if(looping_node->token == ENV)
		{
			env_node = get_env(env, looping_node->content + 1);
			if(!env_node)
				remove_lex_node(mini, looping_node);
			else
			{
				looping_node->content = ft_strdup(env_node->value, GC);
				looping_node->content = get_real_env(looping_node->content);
			}
		}
		looping_node = next;
	}
	
}


char *expanded_line(t_redir *redir, char *line, t_env *env)
{
	t_lex *mini_lexed;
	char *result;

	result = NULL;
	if(redir->here_doc_replacer != REPLACE_ALL)
		return (line);
	mini_lexed = heredoc_tokenizer(line);
	expand_mini_lexed(&mini_lexed, env);
	while(mini_lexed)
	{
		result = ft_strjoin(result, mini_lexed->content, GC);
		mini_lexed = mini_lexed->next;
	}
	return (result);
}


char *get_line_from_buffer(char **buffer)
{
	char *result;
	int i;

	i = 0;

	if (!*buffer || **buffer == '\0')
		return (NULL);
	while ((*buffer)[i] && (*buffer)[i] != '\n')
		i++;
	result = ft_substr(*buffer, 0, i, GC);
	if ((*buffer)[i] == '\n')
		i++;
	*buffer += i;
	return(result);
}

void flag_last_here_doc(t_redir *redir)
{
	t_redir *last;

	last = ft_lstlast_redir(redir);
	while(last)
	{
		if(last->token == HERE_DOC)
		{
			last->actual_here_doc = true;
			break;
		}
		last = last->prev;
	}
}

char	*get_here_doc_path()
{
	int		i;
	char	*str;
	char	*num;

	i = 0;
	while (true)
	{
		num = ft_itoa(i, MANUAL);
		null_protector(num);
		str = ft_strjoin("/tmp/.here_doc", num, GC);
		free(num);
		null_protector(str);
		if (access(str, F_OK) == -1)
			return (str);
		free(str);
		i++;
	}
}

void	handle_red(t_redir *redir, t_treenode *root, t_env **env)
{
	int		fd;
	char	*line;
	char *buffer;
	char *here_doc_path;

	flag_last_here_doc(redir);
	while (redir)
	{
		if (redir->token == REDIR_IN)
		{
			fd = open(redir->redir_string, O_RDONLY, 0777);
			if (fd == -1)
			{
				ft_putstr_fd(2, redir->redir_string);
				perror(": ");
				change_status(env, 1);
				init_tree(root);
				return ;
			}
			store_fds(fd);
			if (dup2(fd, 0) == -1)
			{
				ft_putstr_fd(2, redir->redir_string);
				perror(": ");
				change_status(env, 1);
				init_tree(root);
				return;
			}
			change_status(env, 0);
			close(fd);
			remove_fd_node(fd);
		}
		else if (redir->token == REDIR_OUT || redir->token == DREDIR_OUT)
		{
			if (redir->token == DREDIR_OUT)
				fd = open(redir->redir_string, O_RDWR | O_CREAT | O_APPEND, 0777);
			else if (redir->token == REDIR_OUT)
				fd = open(redir->redir_string, O_RDWR | O_CREAT | O_TRUNC, 0777);
			if (fd == -1)
			{
				ft_putstr_fd(2, redir->redir_string);
				perror(": ");
				change_status(env, 1);
				init_tree(root);
				return;
			}
			store_fds(fd);
			if (dup2(fd, 1) == -1)
			{
				ft_putstr_fd(2, redir->redir_string);
				perror(": ");
				change_status(env, 1);
				init_tree(root);
				return;
			}
			change_status(env, 0);
			close(fd);
			remove_fd_node(fd);
		}
		else if(redir->token == HERE_DOC)
		{
			here_doc_path = get_here_doc_path();
			fd = open(here_doc_path, O_CREAT | O_RDWR | O_TRUNC, 0777);
			if(fd == -1)
			{
				ft_putstr_fd(2, redir->redir_string);
				ft_putstr_fd(2, ": ");
				perror("");
				change_status(env, 1);
				init_tree(root);
				return;
			}
			store_fds(fd);
			if(redir->here_doc_buffer)
			{
				buffer = redir->here_doc_buffer;
				while(true)
				{
					line = get_line_from_buffer(&buffer);
					if(!line)
						break;
					line = expanded_line(redir, line, *env);
					if(line)
						write(fd, line, ft_strlen(line));
					write(fd, "\n", 1);
				}
			}
			close(fd);
			remove_fd_node(fd);
			fd = open(here_doc_path, O_RDONLY);
			if(fd == -1)
			{
				ft_putstr_fd(2, "Heredoc: ");
				perror("");
				change_status(env, 1);
				init_tree(root);
				return;
			}
			store_fds(fd);
			if(redir->actual_here_doc)
			{
				if (dup2(fd, 0) == -1)
				{
					ft_putstr_fd(2, "dup2: ");
					perror("");
					change_status(env, 1);
					init_tree(root);
					return;
				}
			}
			close(fd);
			remove_fd_node(fd);
			unlink(here_doc_path);
		}
		redir = redir->next;
	}
}
