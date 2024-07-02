#include "../minishell.h"

t_lex	*ft_lstnew_lex(char *content, int token, int len, int join_count)
{
	t_lex	*new_node;

	new_node = smart_malloc(sizeof(t_lex));
	new_node->content = content;
	new_node->len = len;
	new_node->token = token;
	new_node->to_replace = REPLACE_ALL;
	new_node->join_count = join_count;
	new_node->here_doc_fd = NONE;
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

// EVERYTHING BELOW HERE IS FOR DEBUGGING PURPOSES AND SHOULD BE REMOVED LATER

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
		case OPEN_PARANTHESE: return "OPEN_PARA";
		case ENV_STAR: return "ENV_STAR";
		case CLOSE_PARANTHESE: return "CLOSE_PARA";
		case NONE: return "NONE";
        default: return ft_strjoin("UNKOWN: ", ft_itoa(t, GC), GC);
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
	// char *c = "False";
	// if(lex->to_replace)
	// 	c = "True";
    // printf("| %-10s | %-15s | %-10d |\n", lex->content, tokenToString(lex->token), lex->len);
    // printf("| %-10s | %-15s | %-10s |\n", "", "", "");
    // printf("|------------|-----------------|------------|\n");
	printf("Content: {%s} // Token: %s // To replace: %i // JC: %i\n", lex->content, tokenToString(lex->token), lex->to_replace, lex->join_count);
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