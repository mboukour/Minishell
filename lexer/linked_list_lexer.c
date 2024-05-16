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
	new_node->state = GENERAL;
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

int counter(t_lex *current, char c)
{
	int count;

	count = 0;
	while(current)
	{
		if(current->len == 1 && current->content[0] == c)
			count++;
		current = current->prev;
	}
	return (count);
}

bool is_behind_in_para(t_lex *current, char c)
{
	while(current)
	{
		if (current->len == 1 && current->content[0] == c && current->state == IN_PARENTHESES)
			return (true);
		current = current->prev;
	}
	return (false);
}
void	ft_lstadd_back_lex(t_lex **lst, t_lex *new)
{
	t_lex	*last_lex;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		new->state = GENERAL;
		return ;
	}
	last_lex = ft_lstlast_lex(*lst);
	last_lex->next = new;
	new->prev = last_lex;
}

const char* tokenToString(enum e_token t)
{
    switch (t) 
	{
        case COMMAND: return "COMMAND";
        case WHITE_SPACE: return "WHITE_SPACE";
        case NEW_LINE: return "NEW_LINE";
        case QUOTE: return "QUOTE";
        case DOUBLE_QUOTE: return "DOUBLE_QUOTE";
        case ENV: return "ENV";
        case PIPE_LINE: return "PIPE_LINE";
        case REDIR_IN: return "REDIR_IN";
        case REDIR_OUT: return "REDIR_OUT";
        case HERE_DOC: return "HERE_DOC";
        case DREDIR_OUT: return "DREDIR_OUT";
		case STAR: return "STAR";
		case AND: return "AND";
		case OR: return "OR";
		case OPEN_PARANTHESE: return "OPEN PARA";
		case CLOSE_PARANTHESE: return "CLOSE PARA";
        default: return "UNKNOWN_TOKEN";
    }
}

const char* stateToString(enum e_state s)
{
    switch (s) 
	{
        case IN_DQUOTE: return "IN_DQUOTE";
        case IN_QUOTE: return "IN_QUOTE";
		case IN_PARENTHESES: return "IN_PARENTHESES";
        case GENERAL: return "GENERAL";
        default: return "UNKNOWN_STATE";
    }
}


void print_lex(const t_lex *lex) {

    // printf("| %-10s | %-15s | %-10d |\n", lex->content, tokenToString(lex->token), lex->len);
    // printf("| %-10s | %-15s | %-10s |\n", "", "", "");
    // printf("|------------|-----------------|------------|\n");
	printf("Content: {%s} // Token: %s\n", lex->content, tokenToString(lex->token));
}

void ft_lstiter_lex(t_lex *lex)
{
    // printf("| %-10s | %-15s | %-10s | \n", "Content", "Token", "Length");
    // printf("|------------|-----------------|------------|\n");
	while(lex)
	{
		print_lex(lex);
		lex = lex->next;
	}
}