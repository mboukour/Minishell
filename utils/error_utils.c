/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:19:40 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/12 17:48:21 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../anti_leaks/cgc.h"
#include "../minishell.h"

void	error_procedure(void)
{
	rl_clear_history();
	free_program();
	smart_close();
	smart_free();
	ft_putstr_fd(2, FAILURE_MSG);
	exit(EXIT_FAILURE);
}
