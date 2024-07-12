/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 20:47:50 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/12 21:25:25 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../minishell.h"

void	safe_free(char **ptr);
void	remove_additional_slashes(char *path);
int		get_i_till(int mode, char **sp_res);
char	*remove_last_slash(char *str);
void	export_wds(char *pwd, char *old_pwd, t_env **env);
void	export_wds(char *pwd, char *old_pwd, t_env **env);
int		check_removed(char *path, t_data *data, t_env **env);
int		handle_directory_change(char *path, t_data *data);
int		check_removed(char *path, t_data *data, t_env **env);
void	cd_error(char *path);
char	*get_new_path(char *pwd, char *original);

#endif