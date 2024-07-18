/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:55:13 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/18 18:44:12 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	remove_env_node(t_env **env, t_env *node_to_remove)
{
	if (node_to_remove == *env)
		*env = node_to_remove->next;
	if (node_to_remove->prev != NULL)
		node_to_remove->prev->next = node_to_remove->next;
	if (node_to_remove->next != NULL)
		node_to_remove->next->prev = node_to_remove->prev;
	free(node_to_remove->value);
	remove_ptr(node_to_remove->value);
	remove_ptr(node_to_remove);
	free(node_to_remove);
}

bool	unset_checker(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isalpha(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

int	unset_core(t_env **env, char *to_unset, t_data *data)
{
	t_env	*unset_env;

	if (!to_unset)
		return (1);
	if (!unset_checker(to_unset))
	{
		ft_putstr_fd(2, "unset: `");
		ft_putstr_fd(2, to_unset);
		ft_putstr_fd(2, "': not a valid identifier\n");
		return (1);
	}
	unset_env = get_env(*env, to_unset);
	if (!unset_env)
	{
		printf("NO\n");
		return (0);
	}
	if (!ft_strcmp("PWD", to_unset))
		data->show_pwd = false;
	else if (!ft_strcmp("OLDPWD", to_unset))
		data->show_oldpwd = false;
	remove_env_node(env, unset_env);
	return (0);
}

int	unset(t_env **env, t_treenode *unset_root, t_data *data)
{
	t_arg	*arg;
	int		r;

	r = 0;
	arg = unset_root->args;
	if (!arg)
		return (0);
	while (arg)
	{
		if (unset_core(env, arg->content, data))
			r = 1;
		arg = arg->next;
	}
	return (r);
}
