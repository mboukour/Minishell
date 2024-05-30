/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabrouk <amabrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 13:17:34 by amabrouk          #+#    #+#             */
/*   Updated: 2024/05/27 17:24:55 by amabrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
#define EXECUTION_H

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

void	pipeline(t_treenode *root, t_env **env);
void ft_parsing(t_treenode *root, t_env *env, t_data *data);
void find_path(t_treenode *root, char **allpath, t_data *data);
#endif