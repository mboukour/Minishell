/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 13:17:34 by amabrouk          #+#    #+#             */
/*   Updated: 2024/07/15 02:16:08 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../minishell.h"
# include <fcntl.h>

# define CMD_NOT_FOUND 127
# define DIRECORY_STATUS 126

void	sigquit_handler_cmd(int sig);

void	pipeline(t_treenode *root, t_data *data, t_env **env);
void	get_path(t_treenode *root, t_env *env, t_data *data);
int		traverse_tree(t_treenode *root, t_data *data, t_env **env);
void	execute_command(t_treenode *root, t_env **env, t_data *data);
void	handle_red(t_redir *redir, t_treenode *root, t_env **env);

int		cd(t_treenode *cd_root, t_env **env, t_data *data);
int		pwd(t_data *data);
int		env(t_env *env);
int		export(t_env **env, t_treenode *export_root);
int		unset(t_env **env, t_treenode *unset_root);
int		echo(t_treenode *echo_root);
int		exit_cmd(t_treenode *root, t_data *data);

void	pipeline(t_treenode *root, t_data *data, t_env **env);
void	get_path(t_treenode *root, t_env *env, t_data *data);
int		traverse_tree(t_treenode *root, t_data *data, t_env **env);
void	execute_command(t_treenode *root, t_env **env, t_data *data);
void	handle_red(t_redir *redir, t_treenode *root, t_env **env);
void	fd_not_open(t_treenode *root, t_redir *redir, t_env **env);
void	ft_heredoc_buffer(t_redir *redir, t_env **env, int fd);
void	ft_dup_heredoc(t_treenode *root, t_redir *redir, char *p, t_env **env);
char	*get_line_from_buffer(char **buffer);
void	flag_last_here_doc(t_redir *redir);
void	init_tree(t_treenode *root);

void	pipeline_error(char *first, int status);
char	*expanded_line(t_redir *redir, char *line, t_env *env);
#endif