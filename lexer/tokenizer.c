/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 03:41:02 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/15 07:04:31 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

bool	is_special(char c)
{
	return (c == '"' || c == '\'' || c == '|' || c == '&' || c == '$'
		|| c == '*' || c == '>' || c == '<' || is_ws(c)
		|| c == '(' || c == ')');
}

void	handle_space(char *input, int *i, t_lex **head)
{
	int		hold;
	char	*content;
	t_lex	*current_node;

	hold = *i;
	while (is_ws(input[*i]))
		(*i)++;
	content = ft_substr(input, hold, *i - hold, GC);
	current_node = ft_lstnew_lex(content, WHITE_SPACE, *i - hold);
	ft_lstadd_back_lex(head, current_node);
}

void	handle_special(char *input, int *i, t_lex **head)
{
	if (is_ws(input[*i]))
		handle_space(input, i, head);
	else if (input[*i] == '>' && input[*i + 1] == '>')
		handle_double_special(input, i, head);
	else if (input[*i] == '<' && input[*i + 1] == '<')
		handle_double_special(input, i, head);
	else if (input[*i] == '|' && input[*i + 1] == '|')
		handle_double_special(input, i, head);
	else if (input[*i] == '&' && input[*i + 1] == '&')
		handle_double_special(input, i, head);
	else if (input[*i] == '$')
		handle_env(input, i, head);
	else if (input[*i] == '*')
		handle_star(input, i, head);
	else
		handle_general_special(input, i, head);
}

t_lex	*tokenizer(char *input)
{
	int		i;
	t_lex	*head;

	head = NULL;
	i = 0;
	while (input[i])
	{
		if (is_special(input[i]))
			handle_special(input, &i, &head);
		else
			handle_word(input, &i, &head);
	}
	return (head);
}
