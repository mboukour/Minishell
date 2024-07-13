/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 20:05:08 by amabrouk          #+#    #+#             */
/*   Updated: 2024/07/13 18:24:46 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	change_status(t_env **env, int new_status)
{
	char	*current_status;
	char	*final;

	current_status = ft_itoa(new_status, MANUAL);
	null_protector(current_status);
	final = ft_strjoin("?=", current_status, MANUAL);
	null_protector(final);
	free(current_status);
	export_core(env, final);
	free(final);
}

void	init_tree(t_treenode *root)
{
	root->after_redir = NULL;
	root->args = NULL;
	root->before_redir = NULL;
	root->builtin = -1;
	root->content = NULL;
	root->left = NULL;
	root->right = NULL;
	root->to_replace = -1;
}

void	remove_lex_node(t_lex **head, t_lex *node)
{
	if (node == NULL || head == NULL || *head == NULL)
		return ;
	if (node == *head)
		*head = node->next;
	if (node->prev != NULL)
		node->prev->next = node->next;
	if (node->next != NULL)
		node->next->prev = node->prev;
}

void	expand_mini_lexed(t_lex **mini, t_env *env)
{
	t_env	*env_node;
	t_lex	*looping_node;
	t_lex	*next;

	looping_node = *mini;
	while (looping_node)
	{
		next = looping_node->next;
		if (looping_node->token == ENV)
		{
			env_node = get_env(env, looping_node->content + 1);
			if (!env_node)
				remove_lex_node(mini, looping_node);
			else
			{
				looping_node->content = ft_strdup(env_node->value, GC);
				looping_node->content = get_real_env(looping_node->content);
			}
		}
		looping_node = next;
	}
}

char	*expanded_line(t_redir *redir, char *line, t_env *env)
{
	t_lex	*mini_lexed;
	char	*result;

	result = NULL;
	if (redir->here_doc_replacer != REPLACE_ALL)
		return (line);
	mini_lexed = heredoc_tokenizer(line);
	expand_mini_lexed(&mini_lexed, env);
	while (mini_lexed)
	{
		result = ft_strjoin(result, mini_lexed->content, GC);
		mini_lexed = mini_lexed->next;
	}
	return (result);
}
