/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   better_access.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 04:02:15 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 04:02:34 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*access_real_path(const char *path, const char *loop_for)
{
	struct dirent	*entry;
	DIR				*dir;
	char			*result;

	dir = opendir(path);
	if (dir == NULL)
		return (NULL);
	while (true)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		if (!ft_strcasecmp(entry->d_name, loop_for))
		{
			result = ft_strjoin(path, "/", GC);
			result = ft_strjoin(result, entry->d_name, GC);
			closedir(dir);
			return (result);
		}
	}
	closedir(dir);
	return (NULL);
}
