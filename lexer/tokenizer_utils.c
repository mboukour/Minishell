/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 03:27:27 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 03:44:33 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	handle_double_special(char *input, int *i, t_lex **head,
	int *join_count)
{
	t_lex	*current_node;
	char	*content;
	int		type;

	content = ft_substr(input, *i, 2, GC);
	if (input[*i] == '>')
		type = DREDIR_OUT;
	else if (input[*i] == '<')
		type = HERE_DOC;
	else if (input[*i] == '|')
		type = OR;
	else if (input[*i] == '&')
		type = AND;
	if (type == AND || type == OR)
		(*join_count)++;
	current_node = ft_lstnew_lex(content, type, 2, *join_count);
	ft_lstadd_back_lex(head, current_node);
	*i += 2;
}

void	handle_star(char *input, int *i, t_lex **head, int *join_count)
{
	t_lex	*current_node;
	int		hold;
	int		c;
	char	*content;

	c = input[*i];
	hold = *i;
	(*i)++;
	while (input[*i] && (input[*i] == '*' || input[*i] == '$'
			|| !is_special(input[*i])) && !is_ws(input[*i]))
		(*i)++;
	content = ft_substr(input, hold, *i - hold, GC);
	current_node = ft_lstnew_lex(content, c, *i - hold, *join_count);
	ft_lstadd_back_lex(head, current_node);
}

void	handle_env(char *input, int *i, t_lex **head, int *join_count)
{
	t_lex	*current_node;
	int		hold;
	int		c;
	char	*content;

	c = input[*i];
	hold = *i;
	(*i)++;
	while (input[*i] && (input[*i] == '*'
			|| !is_special(input[*i])) && !is_ws(input[*i]))
		(*i)++;
	if ((*i) == hold + 1)
		c = WORD;
	content = ft_substr(input, hold, *i - hold, GC);
	current_node = ft_lstnew_lex(content, c, *i - hold, *join_count);
	ft_lstadd_back_lex(head, current_node);
}

void	handle_general_special(char *input, int *i,
	t_lex **head, int *join_count)
{
	t_lex	*current_node;
	char	*content;
	int		type;

	if (input[*i] == '&')
		type = WORD;
	else
	{
		if (input[*i] == '|')
			(*join_count)++;
		type = input[*i];
	}
	content = ft_substr(input, *i, 1, GC);
	current_node = ft_lstnew_lex(content, type, 1, *join_count);
	ft_lstadd_back_lex(head, current_node);
	(*i)++;
}

void	handle_word(char *input, int *i, t_lex **head, int *join_count)
{
	t_lex	*current_node;
	char	*content;
	int		hold;
	int		type;

	hold = *i;
	type = COMMAND;
	while (input[*i] && (input[*i] == '*'
			|| !is_special(input[*i])) && !is_ws(input[*i]))
	{
		if (input[*i] == '*')
			type = STAR;
		(*i)++;
	}
	content = ft_substr(input, hold, *i - hold, GC);
	current_node = ft_lstnew_lex(content, type, *i - hold, *join_count);
	ft_lstadd_back_lex(head, current_node);
}
