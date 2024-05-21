#include "../minishell.h"



t_middle	*ft_lstnew_middle(char *content, char **args, int token)
{
	t_middle	*new_node;

	new_node = smart_malloc(sizeof(t_middle));
	if (!new_node)
		return (NULL);
	new_node->token = token;
	new_node->content = content;
	new_node->args = args;
	new_node->redir_string = NULL;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

t_middle	*ft_lstlast_middle(t_middle *lst)
{
	t_middle	*temp;

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


void	ft_lstadd_back_middle(t_middle **lst, t_middle *new)
{
	t_middle	*last_middle;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last_middle = ft_lstlast_middle(*lst);
	last_middle->next = new;
	new->prev = last_middle;
}

// DEBUGGING FUNCTIONS
void print_middle(t_middle *node)
{
	int i;

	i = 0;
	
	printf("Content: %s // Token: %s  // ", node->content, tokenToString(node->token));
	if(node->args)
	{
		while (node->args[i])
		{
			printf("Arg %i: %s // ", i, node->args[i]);
			i++;
		}
	}
	if(node->token == REDIR_IN)
		printf("Read from: %s // ", node->redir_string);
	else if (node->token == REDIR_OUT || node->token == DREDIR_OUT)
		printf("Write to: %s // ", node->redir_string);
	else if (node->token == HERE_DOC)
		printf("Delimiter: %s // ", node->redir_string);
	printf("\n");
}

void ft_lstiter_middle(t_middle *first)
{
	while(first)
	{
		print_middle(first);
		first = first->next;
	}
}