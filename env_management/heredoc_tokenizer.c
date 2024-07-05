#include "../minishell.h"



void heredoc_handle_env(char *input, int *i, t_lex **head, int *join_count)
{
	t_lex *current_node;
    int hold;
    int c;
    char *content;

    c = input[*i];
	hold = *i;
    (*i)++;
    while (input[*i] && input[*i] != '$' && !is_ws(input[*i])  && input[*i] != '.' && !is_special(input[*i]))
        (*i)++;
    content = ft_substr(input, hold, *i - hold, GC);
    current_node = ft_lstnew_lex(content, c, *i - hold, *join_count);
    ft_lstadd_back_lex(head, current_node);
}


void heredoc_handle_word(char *input, int *i, t_lex **head, int *join_count)
{
	t_lex *current_node;
    char *content;
    int hold;
	int type;

	hold = *i;
	type = COMMAND;
    while (input[*i] && input[*i] != '$')
        (*i)++;
    content = ft_substr(input, hold, *i - hold, GC);
    current_node = ft_lstnew_lex(content, type , *i - hold, *join_count);
    ft_lstadd_back_lex(head, current_node);
}



t_lex *heredoc_tokenizer(char *input)
{
    int i;
    t_lex *head;
	int join_count;

	head = NULL;
	i = 0;
	join_count = 0;
	while (input[i])
    {
        if (input[i] == '$')
            heredoc_handle_env(input, &i, &head, &join_count);
        else
            heredoc_handle_word(input, &i, &head, &join_count);
    }
	return (head);
}