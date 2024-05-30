/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fun2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabrouk <amabrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 16:26:17 by amabrouk          #+#    #+#             */
/*   Updated: 2024/05/27 17:44:44 by amabrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void find_path(t_treenode *root, char **allpath, t_data *data)
{
	char	*joined;
	int		i;

	joined = NULL;
	i = 0;
	while (allpath[i])
	{
		allpath[i] = ft_strjoin(allpath[i], "/", 1);
		joined = ft_strjoin(allpath[i], root->content, 1);
		if (joined && access(joined, F_OK) == 0)
		{
			data->path = ft_strdup(joined, 1);
			return;
		}
		i++;
	}
	data->path = NULL;
}

int	is_whitespace(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] == ' ')
		i++;
	if (i == ft_strlen(s))
		return (1);
	return (0);
}

int	ft_strcmp_nl(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if (s1[i] == 0 && s2[i] && s2[i] == '\n')
		return (0);
	return (1);
}
