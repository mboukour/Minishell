/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_management.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 17:37:19 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/14 01:42:43 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_MANAGEMENT_H
# define ENV_MANAGEMENT_H

# include "../minishell.h"

typedef struct s_exp_vars
{
	bool		go;
	char		*look_for;
	char		*append_after;
	t_lex		*broken_next;
	t_env		*env;
	t_arg		*move;
	t_env		*env_node;
	t_cmd_arg	**cmd_arg;
	t_arg		*arg;
	t_arg		*prev;
	t_arg		*next;
	t_cmd_arg	*expanded_env;
	t_cmd_arg	*last_expanded;
	t_cmd_arg	*looping_cmd;
	t_cmd_arg	*next_lp_cmd;
}	t_exp_vars;

void		expand_node(t_treenode *root, t_env **env);
void		star_expander(t_cmd_arg **cmd_arg);

void		ultimate_expander(t_cmd_arg **cmd_arg, t_env *env);
int			general_expand(t_exp_vars *vars, t_arg **arg, int after_star);
int			specefic_expand(t_exp_vars *vars, t_arg *arg);

int			handle_both_not_joinable(t_exp_vars *vars);
int			handle_both_joinable(t_exp_vars *vars);
int			handle_only_after_joinable(t_exp_vars *vars);
int			handle_only_before_joinable(t_exp_vars *vars);

void		only_joinable_after_prev(t_exp_vars *vars);
void		only_joinable_after_next(t_exp_vars *vars);
void		both_joinable_prev(t_exp_vars *vars);
void		both_joinable_next(t_exp_vars *vars);

void		expand_only_env(t_arg **arg_head, t_env *env);
t_arg		*final_args(t_cmd_arg *cmd_arg);
int			handle_arg_star(t_exp_vars *vars, t_cmd_arg **cmd_arg, t_arg *arg);

t_cmd_arg	*env_to_cmd_arg(t_env *env_node);
bool		is_env(char *str);
char		*get_real_env(char *value);
t_arg		*env_to_arg(t_env *env_node);
int			after_env_star(char *str);
bool		am_i_a_star(char *str);
int			get_to_replace(t_arg *arg);
char		*args_to_str(t_arg *args);
int			get_least_replace(t_arg *args);
void		expand_arg_as_star(t_arg **head);
bool		is_arg_star(t_arg *arg);
bool		is_nextable(t_arg *arg, t_env *env);
char		*lex_to_str(t_lex *lex);
void		handle_splitted_env(t_cmd_arg **head, char *value, t_env *env_node);
void		remove_arg_node(t_arg **head_ref, t_arg *node_to_remove);
void		replace_arg_with_list(t_arg **head_ref, t_arg *node_to_replace,
				t_arg *new_list_head);
void		replace_cmd_arg_node(t_cmd_arg **head, t_cmd_arg *node,
				t_cmd_arg *new_head);
void		remove_cmd_arg_node(t_cmd_arg **head, t_cmd_arg *node);
void		insert_before_cmd_arg(t_cmd_arg **head, t_cmd_arg *node,
				t_cmd_arg *new_node);
void		insert_after_cmd_arg(t_cmd_arg *node, t_cmd_arg *new_node);
bool		is_a_directory(char *path);
bool		is_path(char *str);
char		*no_stars(char *path);

void		ambiguous_redirect(t_arg *redir_input, t_treenode *root,
				t_env **env);

#endif