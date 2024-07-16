/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:53:40 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/16 03:45:22 by mboukour         ###   ########.fr       */
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
			perror("pwd: error retrieving current directory: getcwd: ");
			return (1);
		}
		printf("%s\n", pwd);
		free(pwd);
	}
	return (0);
}
