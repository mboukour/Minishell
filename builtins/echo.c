/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:36:23 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 00:03:57 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_all_n(char *content)
{
	int	i;

	i = 0;
	while (content[i] && content[i] == 'n')
		i++;
	if (content[i])
		return (false);
	return (true);
}

int	echo(t_treenode *echo_root)
{
	t_arg	*args;
	bool	new_line;

	args = echo_root->args;
	new_line = true;
	while (args && args->content && !ft_strncmp(args->content, "-n", 2)
		&& is_all_n(args->content + 2))
	{
		new_line = false;
		args = args->next;
	}
	while (args)
	{
		if (args->content)
		{
			printf("%s", args->content);
			if (args->next)
				printf(" ");
		}
		args = args->next;
	}
	if (new_line)
		printf("\n");
	return (0);
}
