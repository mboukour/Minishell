/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabrouk <amabrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 00:04:39 by amabrouk          #+#    #+#             */
/*   Updated: 2024/05/31 10:47:50 by amabrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	find_path(t_treenode *root, char **allpath, t_data *data)
{
	char	*joined;
	int		i;

	joined = NULL;
	i = 0;
	while (allpath[i])
	{
		allpath[i] = ft_strjoin(allpath[i], "/", GC);
		joined = ft_strjoin(allpath[i], root->content, GC);
		if (joined && access(joined, F_OK) == 0)
		{
			data->path = ft_strdup(joined, 1);
			return;
		}
		i++;
	}
	data->path = NULL;
}

void	get_path(t_treenode *root, t_env *env, t_data *data) // Copy
{
	char	**path;
	t_env	*path_node;

	path_node = get_env(env, "PATH");
	if(!path_node)
	{
		data->path = ft_strdup(root->content, GC);
		return;
	}
	path = ft_split(path_node->value, ':', GC);
	if (root->content[0] == '/' || root->content[0] == '.')
		data->path = ft_strdup(root->content, GC);
	else
		find_path(root, path, data);
}
