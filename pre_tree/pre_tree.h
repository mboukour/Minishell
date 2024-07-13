/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_tree.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 04:53:06 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 04:59:01 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRE_TREE_H
# define PRE_TREE_H

# include "../minishell.h"

bool	is_joinable_lex(t_lex *lex);
void	initialize_vars(t_middle_vars *vars);
void	jump_spaces(t_lex **lex);
t_arg	*make_args(t_lex **first_arg);
t_arg	*make_command(t_lex **lex);
void	process_redirection_token(t_lex **lex, t_middle **head, int token);

#endif