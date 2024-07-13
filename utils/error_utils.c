/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:19:40 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 05:21:52 by mboukour         ###   ########.fr       */
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
void display_error(int error_checker, t_lex *lex, t_env **env)
{
    change_status(env, SYNTAX_ERROR_STATUS);
    if(error_checker != NONE)
    {
        if(error_checker == QUOTE)
            ft_putstr_fd(2, QUOTE_ERROR);
        else
            ft_putstr_fd(2, PARA_ERROR);
        return;
    }
    ft_putstr_fd(2, PARSE_ERROR);
    ft_putstr_fd(2, lex->content);
    ft_putstr_fd(2, "\n");
}
