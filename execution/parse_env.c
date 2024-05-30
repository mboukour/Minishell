/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabrouk <amabrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 00:04:39 by amabrouk          #+#    #+#             */
/*   Updated: 2024/05/30 15:09:45 by amabrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	**ft_parsing(t_treenode *root, t_env *env, t_data *data)
{
	char	**path;
	char	**cmd;
	t_env	*path_node;
	int		i;

	i = 0;
	path_node = get_env(env, "PATH");
	// cmd = get_cmd();												//	TO_DO
	path = ft_split(path_node->value, ':', GC);
	if (root->content[0] == '/' || root->content[0] == '.')
	{
		return (data->path = root->content, &root->content);				// kant return (root->content);
	}
	else
		return (find_path(root, path, data));
	return (perror(NULL), cmd);
}
