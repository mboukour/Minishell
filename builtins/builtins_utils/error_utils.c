/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 00:14:10 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 01:21:32 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../builtins.h"

void	export_error(char *str)
{
	ft_putstr_fd(2, "export: `");
	ft_putstr_fd(2, str);
	ft_putstr_fd(2, "' : not a valid identifier\n");
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
