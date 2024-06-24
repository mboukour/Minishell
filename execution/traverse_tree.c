/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traverse_tree.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabrouk <amabrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 07:51:20 by amabrouk          #+#    #+#             */
/*   Updated: 2024/06/06 15:56:11 by amabrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// REMOVE !!!!


#include "execution.h"

void sigint_handler_cmd(int sig)
{
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
}

char *args_to_str(t_arg *args)
{
	char *result;

	result = NULL;
	while (args)
	{
		printf("%p\n", args);
		if(args->content && args->content[0] == ' ') // TO REMOVEEEE !!!!
			fprintf(stderr, "SPACE\n");
		result = ft_strjoin(result, args->content, GC);
		args = args->next;
	}
	return (result);
}

t_arg *final_args(t_cmd_arg *cmd_arg)
{
	t_arg *args;
	t_arg *head;
	t_arg *current;
	char *ready_arg;

	head = NULL;
	ready_arg = NULL;
	while(cmd_arg)
	{
		args = cmd_arg->arg;
		while(args)
		{
			ready_arg = ft_strjoin(ready_arg, args->content, GC);
			args = args->next;
		}
		current = ft_lstnew_arg(NULL);
		current->content = ready_arg;
		ft_lstaddback_arg(&head, current);
		cmd_arg = cmd_arg->next;
		ready_arg = NULL;
	}
	return (head);
}
				// if (args->to_replace == REPLACE_ALL)
				// {
				// 	star = star_matching(args->content);
				// 	if (star)
				// 	{
				// 		while (star)
				// 		{
				// 			tmp_arg = ft_lstnew_arg(NULL);
				// 			tmp_arg->content = star->value;
				// 			tmp_cmd_arg = ft_lstnew_cmd_arg(tmp_arg);
				// 			if (cmd_arg->prev)
				// 			{
				// 				cmd_arg->prev->next = tmp_cmd_arg;
				// 				tmp_cmd_arg->prev = cmd_arg->prev;
				// 			}
				// 			else
				// 				original = tmp_cmd_arg;
				// 			tmp_cmd_arg->next = cmd_arg->next;
				// 			cmd_arg->prev = tmp_cmd_arg;
				// 			star = star->next;
				// 		}
				// 	}
				// }


// void arg_env_expander(t_cmd_arg **cmd_arg, t_cmd_arg *current, t_arg)
void arg_env_setup(t_cmd_arg **cmd_arg, t_cmd_arg *general_list, t_arg *the_env, t_env *env);
t_arg *expand_args(t_cmd_arg *cmd_arg, t_env *env)
{
    t_arg *args;
    t_cmd_arg *original;
	t_arg *tmp_arg;
    t_cmd_arg *tmp_cmd_arg;
	t_cmd_arg *new_cmd_arg;
    t_arg *hold_args;
    t_env *star;
	t_cmd_arg *add;
	t_cmd_arg *next_cmd_arg;

    original = cmd_arg;
	new_cmd_arg = NULL;
	while (cmd_arg)
	{
		next_cmd_arg = cmd_arg->next;
		args = cmd_arg->arg;
		while (args)
		{
			if (args->to_replace != NO_REPLACE && args->token == ENV)
			{
				arg_env_setup(&original, cmd_arg, args, env);
				break;
			}
			
			args = args->next;
		}
		ft_lstiter_arg(cmd_arg->arg);
		cmd_arg = next_cmd_arg;
	}
    return (final_args(original));
}

// void expand_redirs(t_redir *redir, t_env **env, t_treenode *root)
// {
// 	t_env *star;
// 	t_arg *arg;

// 	while (redir)
// 	{
// 		arg = redir->redir_input;
// 		while(arg)
// 		{
// 			if(arg->to_replace != NO_REPLACE)
// 			{
// 				arg->content = env_expander(arg->content, *env);
// 				if(arg->to_replace == REPLACE_ALL)
// 				{
// 					star = star_matching(arg->content);
// 					if(star)
// 					{
// 						if(star->next)
// 						{
// 							ft_putstr_fd(2, redir->redir_string);
// 							ft_putstr_fd(2, ": ambiguous redirect\n");
// 							change_status(env, 1);
// 							init_tree(root);
// 							return;
// 						}
// 						else
// 							arg->content = ft_strdup(star->value, GC);
// 					}
// 				}
// 			}
// 			arg = arg->next;
// 		}
// 		arg = redir->redir_input;
// 		while(arg)
// 		{
// 			redir->redir_string = ft_strjoin(redir->redir_string, arg->content, GC);
// 			arg = arg->next;
// 		}
// 		redir = redir->next;
// 	}
// }


bool is_a_directory(char *path) 
{
    DIR *dir;

	dir = opendir(path);
    if (dir != NULL) 
	{
        closedir(dir);
        return (true);
    }
    return (false);
}

int count_no_star(char *str)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while(str[i])
	{
		if(str[i] != '*')
			count++;
		i++;
	}
	return (count);
}

bool is_builtin(char *command)
{
	return (!ft_strcmp(command, "env") || !ft_strcmp(command, "echo") || !ft_strcmp(command, "pwd") || !ft_strcmp(command, "export") || !ft_strcmp(command, "unset") || !ft_strcmp(command, "cd") || !ft_strcmp(command, "exit"));
}

char *no_stars(char *path)
{
	int i;
	int j;
	int count;
	char *no_star;

	i = 0;
	j = 0;
	if(!path)
		return (NULL);
	count = count_no_star(path);
	no_star = smart_malloc(count + 1);
	while(path[i])
	{
		if(path[i] != '*')
		{
			no_star[j] = path[i];
			j++;
		}
		i++;
	}
	no_star[j] = '\0';
	return (no_star);
}

			// 	command->content = env_expander(command->content, (*env));
			// no_star = no_stars(command->content);
			// if(command->to_replace == REPLACE_ALL)
			// {
			// 	star = star_matching(command->content);
			// 	if(star)
			// 	{
			// 		command->content = star->value;
			// 		star = star->next;
			// 	}
			// 	while(star)
			// 	{
			// 		tmp_arg = ft_lstnew_arg(NULL);
			// 		tmp_arg->content = ft_strdup((*env)->value, GC);
			// 		ft_lstaddback_arg(&args, tmp_arg);
			// 		star = star->next;
			// 	}
			// }





void expand_node(t_treenode *root, t_env **env)
{
	t_arg *command;
	t_env *star;
	t_arg *args;
	char *no_star;
	t_arg *tmp_arg;

	command = root->command;
	args = NULL;
	tmp_arg = NULL;
	no_star = NULL;
	if(command)
	{
		while(command)
		{
			if(command->to_replace != NO_REPLACE && command->token == ENV)
				better_env_expander(&root->command, &command, *env);
			else
				command = command->next;
		}
		ft_lstiter_arg(root->command);
		root->content = root->command->content;
		tmp_arg = root->command->next;
		root->command->next = NULL;
	}
	root->args = tmp_arg;
	// if(!tmp_arg)
	// 	root->args = expand_args(root->cmd_arg, *env);
	// else
	// {
	// 	tmp_arg->next = expand_args(root->cmd_arg, *env);
	// 	root->args = tmp_arg;
	// }
	// if(no_star && !is_builtin(no_star) && is_a_directory(no_star))
	// {
	// 	ft_putstr_fd(2, no_star);
	// 	ft_putstr_fd(2,": Is a directory\n");
	// 	root->is_a_directory = true;
	// 	root->content = ft_strdup(no_star, GC);
	// 	change_status(env, DIRECORY_STATUS);
	// 	return;
	// }
	// expand_redirs(root->before_redir, env, root);
	// expand_redirs(root->after_redir, env, root);
}


int	traverse_tree(t_treenode *root, t_data *data, t_env **env)
{
	int		save_in;
	int		save_out;

	save_in = dup(0);
	save_out = dup(1);
	signal(SIGINT, sigint_handler_cmd);
	if (!root)
		return (0);
	if(root->token != AND  && root->token != OR && root->token != PIPE_LINE)
		expand_node(root, env);
	if (root->before_redir)
		handle_red(root->before_redir, root, env);
	if (root->after_redir)
		handle_red(root->after_redir, root, env);
	if (root->token == COMMAND)
		execute_command(root, env, data);
	else if (root->token == PIPE_LINE)
		pipeline(root, data, env);
	else if (root->token == AND)
	{
		if (traverse_tree(root->left, data, env) == 0 || data->status == 0)
			traverse_tree(root->right, data, env);
	}
	else if (root->token == OR)
	{
		if (traverse_tree(root->left, data, env) == 0 || data->status != 0)
			traverse_tree(root->right, data, env);
	}
	else
		fprintf(stderr, "ERROR: %i\n", root->token); // REMOVE THIS: DUBGGING
	dup2(save_in, 0);
	dup2(save_out, 1);
	close(save_in);
	close(save_out);
	return 1;
}
