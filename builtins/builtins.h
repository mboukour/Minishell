/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 20:47:50 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/18 18:14:51 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../minishell.h"

void	safe_free(char **ptr);
void	remove_additional_slashes(char *path);
int		get_i_till(int mode, char **sp_res);
char	*remove_last_slash(char *str);
void	export_wds(char *pwd, t_env **env, t_data *data);
int		check_removed(char *path, t_data *data, t_env **env, t_arg *cd_args);
int		handle_directory_change(char *path, t_data *data, t_env **env, t_arg *cd_args);
void	cd_error(char *path);
char	*get_new_path(char *pwd, char *original);
bool	no_equal(char *str);
bool	underscore_before_equal(char *str);
bool	initial_check(char *str, int i, int count);
int		is_append(char *str);
int		get_export_type(char *str, int i);
char	*get_look_for(char *str);
char	*get_append_result(char *exp_arg, char *tmp);
void	add_new_env(t_env **env, char *exp_arg, int exp_type);
void	update_existing_env(t_env *find, char *final);
void	export_error(char *str);
bool	unset_checker(char *str);
int		cd_home_on_no_dir(t_env *env, t_data *data);
void	ft_lstiter_env(t_env *env, t_data *data);
#endif