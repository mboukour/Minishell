/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:45:36 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/12 17:50:14 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "../minishell.h"

t_redir		*handle_before_redirs(t_middle **middled);
t_redir		*handle_after_redirs(t_middle **middled);
t_treenode	*new_treenode(t_middle *middled);
void		nullify_all(t_treenode *node);
t_arg		*copy_arg(t_arg *arg);
t_redir		*ft_lstnew_redir(t_middle *middled);
bool		valid_parse_cmd_middle(t_middle *middled);

#endif