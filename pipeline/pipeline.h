/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabrouk <amabrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 13:17:34 by amabrouk          #+#    #+#             */
/*   Updated: 2024/05/26 18:54:01 by amabrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPELINE_H
#define PIPELINE_H

# include "../minishell.h"
# include <fcntl.h>

typedef struct s_treenode t_treenode;
typedef struct s_env t_env;

typedef struct s_data
{
	int		infile;
	int		outfile;
	int		end[2];
	char	**cmd;
	char	*path;
}				t_data;

void	ft_pipe(t_treenode *root, t_env **env);
char	**ft_parsing(t_treenode *root, char	*content, t_env *env, t_data *data);
char	**find_path(t_treenode *root, char **path, char **cmd, t_data *data);

#endif