/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 20:47:27 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/18 17:57:04 by mboukour         ###   ########.fr       */
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

void	export_wds(char *pwd, t_env **env, t_data *data)
{
	char	*result;
	t_env	*env_old;

	env_old = get_env(*env, "PWD");
	if (env_old)
	{
		result = get_real_env(env_old->value);
		result = ft_strjoin("OLDPWD=", result, MANUAL);
		null_protector(result);
		export_core(env, result, data);
		free(result);
	}
	result = ft_strjoin("PWD=", pwd, MANUAL);
	null_protector(result);
	export_core(env, result, data);
	free(result);
}
int	cd_home_on_no_dir(t_env *env, t_data *data)
{
	t_env	*home_env;
	home_env = get_env(env, "HOME");
	if (!home_env)
	{
		ft_putstr_fd(2, "cd: HOME not set\n");
		return (1);
	}
	if(chdir(home_env->value + 5))
		return (cd_error(home_env->value + 5), 1);
	data->pwd = getcwd(NULL, 0);
	return (0);
}