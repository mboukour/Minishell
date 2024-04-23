#include "../minishell.h"

// echo "hello  $USER " > file | grep h | cat << eof | cat >> file | echo 'done'

bool is_special(char c) 
{
    return (c == '\'' || c == '"' || c == '|' || c == '$' || c == '&' || c == ';' || c == '*' || c == '\\' || c == '>' || c == '<' || c == ' ');
}


t_lex *lexer(char *input)
{
	int i;
	int hold;
	t_lex *current_node;
	t_lex *head;
	char *content;

	i = 0;
	head = NULL;
	while(input[i])
	{
		if(is_special(input[i]))
		{
			if(input[i] == ' ')
			{
				hold = i;
				while(input[i] == ' ')
					i++;
				content = ft_substr(input, hold, i - hold);
				current_node = ft_lstnew_lex(content, WHITE_SPACE, i - hold);
				ft_lstadd_back_lex(&head, current_node);
			}
			else if (input[i] == '>' && input[i + 1] && input[i + 1] == '>')
			{
				content = ft_substr(input, i, 2);
				current_node = ft_lstnew_lex(content, DREDIR_OUT, i - hold);
				ft_lstadd_back_lex(&head, current_node);
				i += 2;
			}
			else if (input[i] == '<' && input[i + 1] && input[i + 1] == '<')
			{
				content = ft_substr(input, i, 2);
				current_node = ft_lstnew_lex(content, HERE_DOC, i - hold);
				ft_lstadd_back_lex(&head, current_node);
				i += 2;
			}
			else if (input[i] == '$')
			{
				hold = i;
				i++;
				while(input[i] && !is_special(input[i]) && input[i] != ' ')
					i++;
				content = ft_substr(input, hold, i - hold);
				current_node = ft_lstnew_lex(content, ENV, i - hold);
				ft_lstadd_back_lex(&head, current_node);
			}
			else
			{
				content = ft_substr(input, i, 1);
				current_node = ft_lstnew_lex(content, input[i], 1);
				ft_lstadd_back_lex(&head, current_node);
				i++;
			}
		}
		else
		{
			hold = i;
			while(input[i] && !is_special(input[i]) && input[i] != ' ')
				i++;
			content = ft_substr(input, hold, i - hold);
			current_node = ft_lstnew_lex(content, WORD, i - hold);
			ft_lstadd_back_lex(&head, current_node);
		}
	}
	return (head);
}