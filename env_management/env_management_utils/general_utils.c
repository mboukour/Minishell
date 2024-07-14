/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 00:53:15 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/14 01:39:42 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../env_management.h"

t_cmd_arg	*env_to_cmd_arg(t_env *env_node)
{
	t_cmd_arg	*head;
	char		**sp_res;
	char		*real_env;
	int			i;

	head = NULL;
	i = 0;
	real_env = get_real_env(env_node->value);
	sp_res = ft_split_ws(real_env, GC);
	if (sp_res)
	{
		while (sp_res[i])
		{
			handle_splitted_env(&head, sp_res[i], env_node);
			i++;
		}
	}
	else
	{
		head = ft_lstnew_cmd_arg(ft_lstnew_arg(NULL));
		head->arg->content = ft_strdup(env_node->value, GC);
	}
	return (head);
}

void	ambiguous_redirect(t_arg *redir_input, t_treenode *root, t_env **env)
{
	ft_putstr_fd(2, args_to_str(redir_input));
	ft_putstr_fd(2, ": ambiguous redirect\n");
	export_core(env, "?=1");
	init_tree(root);
	return ;
}
