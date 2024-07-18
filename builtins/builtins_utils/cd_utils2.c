/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 20:50:40 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/18 18:10:26 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../builtins.h"

bool	is_all_points(char *path)
{
	int	i;
	int	dot_count;

	i = 0;
	dot_count = 0;
	while (path[i])
	{
		if (path[i] != '.' && path[i] != '/')
			return (false);
		if (path[i] == '.')
		{
			dot_count++;
			if (dot_count > 2)
				return (false);
		}
		else if (path[i] == '/')
			dot_count = 0;
		i++;
	}
	if (dot_count > 2)
		return (false);
	return (true);
}

char	*add_slash_if_needed(char *path)
{
	int	i;

	i = 0;
	while (path[i])
		i++;
	if (path[i] != '/')
	{
		remove_ptr(path);
		path = ft_strjoin(path, "/", GC);
	}
	else
		path = ft_strdup(path, GC);
	return (path);
}

int	handle_directory_change(char *path, t_data *data, t_env **env,t_arg *cd_args)
{
	char	*new;

	new = get_new_path(data->pwd, path);
	if (!chdir(new))
	{
		if (remove_ptr(data->pwd))
			safe_free(&data->pwd);
		data->pwd = ft_strdup(new, MANUAL);
		store_malloced(data->pwd);
		return (2);
	}
	else if (is_all_points(path))
	{
		data->pwd = ft_strjoin(add_slash_if_needed(data->pwd), path, MANUAL);
		store_malloced(data->pwd);
		cd_error(NULL);
	}
	else if (!cd_args)
		return (cd_home_on_no_dir(*env, data), 2);
	else
		ft_putstr_fd(2, "cd: No such file or directory\n");
	return (1);
}

int	check_removed(char *path, t_data *data, t_env **env, t_arg *cd_args)
{
	char	*dir;
	int		r;

	if (!data->pwd)
		return (3);
	dir = getcwd(NULL, 0);
	r = 0;
	if (!dir)
	{
		r = handle_directory_change(path, data, env, cd_args);
		export_wds(data->pwd, env, data);
	}
	safe_free(&dir);
	return (r);
}

char	*remove_last_slash(char *str)
{
	int		i;
	int		last_slash_index;
	char	*result;

	i = 0;
	last_slash_index = NONE;
	while (str[i])
	{
		if (str[i] == '/')
			last_slash_index = i;
		i++;
	}
	if (last_slash_index == -1)
		return (str);
	result = smart_malloc(last_slash_index + 1);
	ft_strncpy(result, str, last_slash_index + 1);
	result[last_slash_index] = '\0';
	return (result);
}
