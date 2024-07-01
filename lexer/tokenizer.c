#include "../minishell.h"


bool is_special(char c)
{
    return (c == '\'' || c == '"' || c == '|' || c == '$' || c == '&' ||  c == '*' || c == '>' || c == '<' || c == ' ' || c == '*' || c == '(' || c == ')');
}

void handle_double_special(char *input, int *i, t_lex **head, int type, int *join_count)
{
	t_lex *current_node;
    char *content;

    content = ft_substr(input, *i, 2, GC);
    if(type == AND || type == OR)
        (*join_count)++;
    current_node = ft_lstnew_lex(content, type, 2, *join_count);
    ft_lstadd_back_lex(head, current_node);
    *i += 2;
}

void handle_star(char *input, int *i, t_lex **head, int *join_count)
{
	t_lex *current_node;
    int hold;
    int c;
    char *content;

    c = input[*i];
	hold = *i;
    (*i)++;
    while (input[*i] && (input[*i] == '*' || input[*i] == '$' || !is_special(input[*i])) && input[*i] != ' ')
        (*i)++;
    content = ft_substr(input, hold, *i - hold, GC);
    current_node = ft_lstnew_lex(content, c, *i - hold, *join_count);
    ft_lstadd_back_lex(head, current_node);
}

void handle_env(char *input, int *i, t_lex **head, int *join_count)
{
	t_lex *current_node;
    int hold;
    int c;
    char *content;
    bool star_bool;

    star_bool = false;
    c = input[*i];
	hold = *i;
    (*i)++;
    while (input[*i] && (input[*i] == '*' || !is_special(input[*i])) && input[*i] != ' ')
        (*i)++;
    content = ft_substr(input, hold, *i - hold, GC);
    current_node = ft_lstnew_lex(content, c, *i - hold, *join_count);
    ft_lstadd_back_lex(head, current_node);
}

void handle_general_special(char *input, int *i, t_lex **head, int type, int *join_count)
{
	t_lex *current_node;
    char *content;

    if(*input == '&')
        type = WORD;
    else if(*input == '|')
        (*join_count)++;
	content = ft_substr(input, *i, 1, GC);
    current_node = ft_lstnew_lex(content, type, 1, *join_count);
    ft_lstadd_back_lex(head, current_node);
    (*i)++;
}

void handle_word(char *input, int *i, t_lex **head, int *join_count)
{
	t_lex *current_node;
    char *content;
    int hold;
	int type;

	hold = *i;
	type = COMMAND;
    while (input[*i] && (input[*i] == '*' || !is_special(input[*i])) && input[*i] != ' ')
	{
		if(input[*i] == '*')
			type = STAR;
        (*i)++;
	}
    content = ft_substr(input, hold, *i - hold, GC);
    current_node = ft_lstnew_lex(content, type , *i - hold, *join_count);
    ft_lstadd_back_lex(head, current_node);
}

void handle_space(char *input, int *i, t_lex **head, int *join_count)
{
    int hold;
    char *content;
    t_lex *current_node;
    
	hold = *i;
    (*join_count)++;
    while (input[*i] == ' ')
        (*i)++;
    content = ft_substr(input, hold, *i - hold, GC);
    current_node = ft_lstnew_lex(content, WHITE_SPACE, *i - hold, *join_count);
    ft_lstadd_back_lex(head, current_node);
}


void handle_special(char *input, int *i, t_lex **head, int *join_count)
{
    if (input[*i] == ' ')
        handle_space(input, i, head, join_count);
    else if (input[*i] == '>' && input[*i + 1] == '>')
        handle_double_special(input, i, head, DREDIR_OUT, join_count);
    else if (input[*i] == '<' && input[*i + 1] == '<')
        handle_double_special(input, i, head, HERE_DOC, join_count);
    else if (input[*i] == '|' && input[*i + 1] == '|')
        handle_double_special(input, i, head, OR, join_count);
    else if (input[*i] == '&' && input[*i + 1] == '&')
        handle_double_special(input, i, head, AND, join_count);
    else if (input[*i] == '$')
        handle_env(input, i, head, join_count);
    else if (input[*i] == '*')
        handle_star(input, i, head, join_count);
    else
        handle_general_special(input, i, head, input[*i], join_count);
}


t_lex *tokenizer(char *input)
{
    int i;
    t_lex *head;
    char *content;
	int join_count;

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
    return head;
}
