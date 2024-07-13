/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 03:41:02 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 03:43:01 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

bool	is_special(char c)
{
	return (c == '\'' || c == '"' || c == '|' || c == '$' || c == '&'
		|| c == '*' || c == '>' || c == '<' || is_ws(c) || c == '*'
		|| c == '(' || c == ')');
}

void	handle_space(char *input, int *i, t_lex **head, int *join_count)
{
	int		hold;
	char	*content;
	t_lex	*current_node;

	hold = *i;
	(*join_count)++;
	while (is_ws(input[*i]))
		(*i)++;
	content = ft_substr(input, hold, *i - hold, GC);
	current_node = ft_lstnew_lex(content, WHITE_SPACE, *i - hold, *join_count);
	ft_lstadd_back_lex(head, current_node);
}

void	handle_special(char *input, int *i, t_lex **head, int *join_count)
{
	if (is_ws(input[*i]))
		handle_space(input, i, head, join_count);
	else if (input[*i] == '>' && input[*i + 1] == '>')
		handle_double_special(input, i, head, join_count);
	else if (input[*i] == '<' && input[*i + 1] == '<')
		handle_double_special(input, i, head, join_count);
	else if (input[*i] == '|' && input[*i + 1] == '|')
		handle_double_special(input, i, head, join_count);
	else if (input[*i] == '&' && input[*i + 1] == '&')
		handle_double_special(input, i, head, join_count);
	else if (input[*i] == '$')
		handle_env(input, i, head, join_count);
	else if (input[*i] == '*')
		handle_star(input, i, head, join_count);
	else
		handle_general_special(input, i, head, join_count);
}

t_lex	*tokenizer(char *input)
{
	int		i;
	t_lex	*head;
	int		join_count;

	head = NULL;
	i = 0;
	join_count = 0;
	while (input[i])
	{
		if (is_special(input[i]))
			handle_special(input, &i, &head, &join_count);
		else
			handle_word(input, &i, &head, &join_count);
	}
	return (head);
}
