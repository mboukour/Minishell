/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabrouk <amabrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 00:04:39 by amabrouk          #+#    #+#             */
/*   Updated: 2024/05/26 18:54:43 by amabrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline.h"

// char **ft_lstchar(t_arg *args)
// {
// 	int		size;
// 	char	*cmd_options;
// 	int		i;
// 	t_arg	*tmp;

// 	i = 0;
// 	size = ft_lstsize(args);
// 	cmd_options = smart_malloc(size + 1);
// 	while (i < size)
// 	{
// 		cmd_options[i++] = tmp;
// 		tmp = tmp->next;
// 	}
// 	cmd_options[i] = 0;
// 	return (cmd_options);
// }

// char	*ft_strstr(t_env *env, char *find)
// {
// 	int	i;
// 	int	k;

// 	while (env)
// 	{
// 		i = 0;
// 		k = 0;
// 		while (find[k] && env && env == find[k])
// 			k++;
// 		if (!find[k])
// 			return (env->value[i] + 5);
// 		env = env->next;
// 	}
// 	return (NULL);
// }

char	**ft_parsing(t_treenode *root, char	*content, t_env *env, t_data *data)
{
	char	**path;
	char	**cmd;
	t_env	*path_node;
	int		i;

	// if ((av && !av[0]) || is_whitespace(av))
	// {
	// 	write(2, "\tcommand not found\n", 19);
	// 	exit(EXIT_FAILURE);
	// }
	i = 0;
	// cmd = split_option(av);
	// root->content;
	path_node = get_env(env, "PATH");
	// if (path_node)
	// 	path = &path_node->value;
	// s = ft_strstr(env, "PATH=");
	// /bin/ls
	// ["ls", NULL]
	// NULL
	// cmd = get_cmd();												//	TO_DO
	path = ft_split(path_node->value, ':', GC);
	if (content[0] == '/' || content[0] == '.')
	{
		return (data->path = content, &root->content);				// kant return (root->content);
	}
	else
		return (find_path(root, path, &root->content, data));
	return (perror(NULL), cmd);
}
