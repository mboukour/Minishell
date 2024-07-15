/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 21:16:55 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/15 06:43:16 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

char	*get_pointless_path(char *pwd)
{
	char	**sp_res;
	char	*path;
	int		i;
	int		j;

	i = 0;
	j = 0;
	path = "/";
	sp_res = ft_split(pwd, '/', GC);
	i = get_i_till(1, sp_res);
	while (j < i)
	{
		path = ft_strjoin(path, sp_res[j], GC);
		path = ft_strjoin(path, "/", GC);
		j++;
	}
	remove_additional_slashes(path);
	i = get_i_till(0, sp_res);
	while (sp_res[i] && (!ft_strcmp(sp_res[i], "..")
			|| !ft_strcmp(sp_res[i], ".")))
	{
		if (ft_strlen(sp_res[i--]) == 2)
			path = remove_last_slash(path);
	}
	return (path);
}

char	*get_new_path(char *pwd, char *original)
{
	int		i;
	char	**sp_res;
	char	*path;

	path = get_pointless_path(pwd);
	i = 0;
	sp_res = ft_split(original, '/', GC);
	if (!sp_res)
		return (NULL);
	while (sp_res[i])
	{
		if (!ft_strcmp(sp_res[i], ".."))
			path = remove_last_slash(path);
		else
		{
			path = ft_strjoin(path, "/", GC);
			path = ft_strjoin(path, sp_res[i], GC);
		}
		i++;
	}
	return (path);
}

int	cd_core(char *path, t_env **env, t_data *data)
{
	char	*wd;
	int		check;

	check = check_removed(path, data, env);
	if (check == 1)
		return (1);
	else if (check == 2)
		return (0);
	if (chdir(path))
		return (cd_error(path), 1);
	wd = getcwd(NULL, 0);
	if (!wd)
		return (perror("cd: "), 1);
	data->pwd = ft_strdup(wd, MANUAL);
	free(wd);
	store_malloced(data->pwd);
	export_wds(data->pwd, env);
	return (0);
}

int	cd_home(t_env **env, t_data *data)
{
	t_env	*home_env;

	home_env = get_env(*env, "HOME");
	if (!home_env)
	{
		ft_putstr_fd(2, "cd: HOME not set\n");
		return (1);
	}
	return (cd_core(home_env->value + 5, env, data));
}

int	cd(t_treenode *cd_root, t_env **env, t_data *data)
{
	t_arg	*args;

	args = cd_root->args;
	if (!args)
		return (cd_home(env, data));
	return (cd_core(args->content, env, data));
}
