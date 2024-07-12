/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 20:47:27 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/12 21:24:55 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../builtins.h"

void	safe_free(char **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	cd_error(char *path)
{
	if (!path)
	{
		ft_putstr_fd(2, "cd: error retrieving current directory: ");
		ft_putstr_fd(2, "getcwd: cannot access parent directories: ");
		perror("");
		return ;
	}
	ft_putstr_fd(2, "cd: ");
	ft_putstr_fd(2, path);
	ft_putstr_fd(2, " no such file or directory\n");
}

void	remove_additional_slashes(char *path)
{
	int	i;

	i = 0;
	while (path[i])
		i++;
	i--;
	while (path[i] == '/')
		path[i--] = '\0';
}

int	get_i_till(int mode, char **sp_res)
{
	int	i;

	i = 0;
	if (mode == 0)
	{
		while (sp_res[i])
			i++;
		i--;
	}
	else
	{
		while (sp_res[i] && ft_strcmp(sp_res[i], "..")
			&& ft_strcmp(sp_res[i], "."))
			i++;
	}
	return (i);
}

void	export_wds(char *pwd, char *old_pwd, t_env **env)
{
	char	*result;

	result = ft_strjoin("PWD=", pwd, MANUAL);
	null_protector(result);
	export_core(env, result);
	free(result);
	result = ft_strjoin("OLDPWD=", old_pwd, MANUAL);
	null_protector(result);
	export_core(env, result);
	free(result);
}
