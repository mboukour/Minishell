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
	int		i;
	char *real_path;


	i = 0;
	while (allpath[i])
	{
		real_path = access_real_path(allpath[i], root->content);
		if (real_path)
		{
			data->path = real_path;
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
	path = ft_split(get_real_env(path_node->value), ':', GC);
	if (root->content[0] == '/' || root->content[0] == '.')
		data->path = ft_strdup(root->content, GC);
	else
		find_path(root, path, data);
}
