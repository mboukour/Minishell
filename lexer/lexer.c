#include "../minishell.h"

// echo "hello  $USER " > file | grep h | cat << eof | cat >> file | echo 'done'

bool is_special(char c) 
{
    return (c == '\'' || c == '"' || c == '|' || c == '$' || c == '&' ||  c == '*' || c == '\\' || c == '>' || c == '<' || c == ' ' || c == '*' || c == '(' || c == ')');
}

void handle_double_special(char *input, int *i, t_lex **head, char *content, int type)
{
	t_lex *current_node;

    content = ft_substr(input, *i, 2);
    current_node = ft_lstnew_lex(content, type, 2);
    ft_lstadd_back_lex(head, current_node);
    *i += 2;
}

void handle_env(char *input, int *i, t_lex **head)
{
	t_lex *current_node;
    int hold;

	hold = *i;
    (*i)++;
    while (input[*i] && !is_special(input[*i]) && input[*i] != ' ')
        (*i)++;
    char *content = ft_substr(input, hold, *i - hold);
    current_node = ft_lstnew_lex(content, ENV, *i - hold);
    ft_lstadd_back_lex(head, current_node);
}

void handle_general_special(char *input, int *i, t_lex **head, int type)
{
	t_lex *current_node;
    char *content;

	content = ft_substr(input, *i, 1);
    current_node = ft_lstnew_lex(content, type, 1);
    ft_lstadd_back_lex(head, current_node);
    (*i)++;
}

void handle_word(char *input, int *i, t_lex **head)
{
	t_lex *current_node;
    int hold;

	hold = *i;
    while (input[*i] && !is_special(input[*i]) && input[*i] != ' ')
        (*i)++;
    char *content = ft_substr(input, hold, *i - hold);
    current_node = ft_lstnew_lex(content, WORD, *i - hold);
    ft_lstadd_back_lex(head, current_node);
}

void handle_space(char *input, int *i, t_lex **head)
{
    int hold;

	hold = *i;
    while (input[*i] == ' ')
        (*i)++;
    char *content = ft_substr(input, hold, *i - hold);
    t_lex *current_node = ft_lstnew_lex(content, WHITE_SPACE, *i - hold);
    ft_lstadd_back_lex(head, current_node);
}

void handle_special(char *input, int *i, t_lex **head)
{
    if (input[*i] == ' ')
        handle_space(input, i, head);
    else if (input[*i] == '>' && input[*i + 1] == '>')
        handle_double_special(input, i, head, NULL, DREDIR_OUT);
    else if (input[*i] == '<' && input[*i + 1] == '<')
        handle_double_special(input, i, head, NULL, HERE_DOC);
    else if (input[*i] == '|' && input[*i + 1] == '|')
        handle_double_special(input, i, head, NULL, OR);
    else if (input[*i] == '&' && input[*i + 1] == '&')
        handle_double_special(input, i, head, NULL, AND);
    else if (input[*i] == '$')
        handle_env(input, i, head);
    else
        handle_general_special(input, i, head, input[*i]);
}


t_lex *lexer(char *input)
{
    int i;
    t_lex *head;
    char *content;

	head = NULL;
	i = 0;
    while (input[i])
    {
        if (is_special(input[i]))
            handle_special(input, &i, &head);
        else
            handle_word(input, &i, &head);
    }
    return head;
}
