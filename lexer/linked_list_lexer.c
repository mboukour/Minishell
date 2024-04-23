#include "../minishell.h"

t_lex	*ft_lstnew_lex(char *content, int token, int len)
{
	t_lex	*new_node;

	new_node = malloc(sizeof(t_lex));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->len = len;
	new_node->token = token;
	new_node->state = -1;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}


t_lex	*ft_lstlast_lex(t_lex *lst)
{
	t_lex	*temp;

	if (lst == NULL)
		return (NULL);
	temp = lst;
	while (temp)
	{
		if (!temp->next)
			return (temp);
		temp = temp->next;
	}
	return (lst);
}

t_lex	*ft_lstfirst_lex(t_lex *lst)
{
	t_lex	*temp;

	temp = lst;
	while (temp)
	{
		if (!temp->prev)
			return (temp);
		temp = temp->prev;
	}
	return (lst);
}



void	ft_lstadd_back_lex(t_lex **lst, t_lex *new)
{
	t_lex	*last_lex;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last_lex = ft_lstlast_lex(*lst);
	last_lex->next = new;
	new->prev = last_lex;
}

const char* tokenToString(token t) {
    switch (t) {
        case WORD: return "WORD";
        case WHITE_SPACE: return "WHITE_SPACE";
        case NEW_LINE: return "NEW_LINE";
        case QUOTE: return "QUOTE";
        case DOUBLE_QUOTE: return "DOUBLE_QUOTE";
        case ESCAPE: return "ESCAPE";
        case ENV: return "ENV";
        case PIPE_LINE: return "PIPE_LINE";
        case REDIR_IN: return "REDIR_IN";
        case REDIR_OUT: return "REDIR_OUT";
        case HERE_DOC: return "HERE_DOC";
        case DREDIR_OUT: return "DREDIR_OUT";
        default: return "UNKNOWN_TOKEN";
    }
}


void print_lex(const t_lex *lex) {
    printf("Content: %s\n", lex->content);
    printf("Length: %d\n", lex->len);
	printf("Token: %s\n", tokenToString(lex->token));
    // printf("State: %d\n", lex->state);
	printf("----------------------\n");
}

void ft_lstiter_lex(t_lex *lex)
{
	while(lex)
	{
		print_lex(lex);
		lex = lex->next;
	}
}