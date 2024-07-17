/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:53:40 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/17 11:04:37 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

char	*get_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (NULL);
	return (pwd);
}

int	pwd(t_data *data)
{
	char	*pwd;

	if (data->pwd)
		printf("%s\n", data->pwd);
	else
	{
		pwd = get_pwd();
		if (!pwd)
		{
			ft_putstr_fd(2, "error retrieving current directory: ");
			ft_putstr_fd(2, "getcwd: cannot access parent directories: ");
			perror("");
			return (1);
		}
		printf("%s\n", pwd);
		free(pwd);
	}
	return (0);
}
