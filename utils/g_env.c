/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_env.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 22:16:27 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/14 23:24:42 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*globalizer_env(int mode, void *env)
{
	static void	*head;

	if (mode == SET)
		head = env;
	else
		return (head);
	return (NULL);
}
