/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabrouk <amabrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 13:17:34 by amabrouk          #+#    #+#             */
/*   Updated: 2024/05/27 17:24:55 by amabrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
#define EXECUTION_H

# include "../minishell.h"
# include <fcntl.h>

# define CMD_NOT_FOUND 127
# define DIRECORY_STATUS 126


void	pipeline(t_treenode *root, t_data *data, t_env **env);
void	get_path(t_treenode *root, t_env *env, t_data *data);
int		traverse_tree(t_treenode *root, t_data *data, t_env **env);
void	execute_command(t_treenode *root, t_env **env, t_data *data);
void	handle_red(t_redir *redir, t_treenode *root, t_env **env);

int	cd(t_treenode *cd_root, t_env **env, t_data *data);
int pwd(t_data *data);
int env(t_env *env);
char *get_pwd(void); // GC does not free this!
int export(t_env **env, t_treenode *export_root);
int unset(t_env **env, t_treenode *unset_root);
int echo(t_treenode *echo_root);
int exit_cmd(t_treenode *root);



#endif