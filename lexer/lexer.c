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
				printf("(%s) is %i spaces!\n", content, i - hold);
			}
			else if (input[i] == '>' && input[i + 1] && input[i + 1] == '>')
			{
				content = ft_substr(input, i, 2);
				printf("(%s) is %i\n", content, DREDIR_OUT);
				i += 2;
			}
			else if (input[i] == '<' && input[i + 1] && input[i + 1] == '<')
			{
				content = ft_substr(input, i, 2);
				printf("(%s) is %i\n", content, HERE_DOC);
				i += 2;
			}
			else
			{
				content = ft_substr(input, i, 1);
				printf("(%s) is %i\n", content, input[i]);
				i++;
			}
		}
		else
		{
			hold = i;
			while(input[i] && !is_special(input[i]) && input[i] != ' ')
				i++;
			content = ft_substr(input, hold, i - hold);
			printf("(%s) is a word!\n", content);
		}
	}
	return (NULL);
}