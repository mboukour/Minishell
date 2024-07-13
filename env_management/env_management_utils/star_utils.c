/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   star_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 18:03:00 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 18:17:14 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../env_management.h"

bool	is_a_directory(char *path)
{
	DIR		*dir;

	dir = opendir(path);
	if (dir != NULL)
	{
		closedir(dir);
		return (true);
	}
	return (false);
}

int	count_no_star(char *str)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] != '*')
			count++;
		i++;
	}
	return (count);
}

bool	is_path(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '/')
			return (true);
		i++;
	}
	return (false);
}

char	*no_stars(char *path)
{
	int		i;
	int		j;
	int		count;
	char	*no_star;

	i = 0;
	j = 0;
	if (!path)
		return (NULL);
	count = count_no_star(path);
	no_star = smart_malloc(count + 1);
	while (path[i])
	{
		if (path[i] != '*')
		{
			no_star[j] = path[i];
			j++;
		}
		i++;
	}
	no_star[j] = '\0';
	return (no_star);
}
