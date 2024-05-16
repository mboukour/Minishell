#include "../minishell.h"



t_middle	*ft_lstnew_middle(char *content, char **args, int token)
{
	t_middle	*new_node;

	new_node = malloc(sizeof(t_middle));
	if (!new_node)
		return (NULL);
	new_node->token = token;
	new_node->content = content;
	new_node->is_in_para = false;
	new_node->condition_count = 0;
	new_node->args = args;
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


void print_middle(t_middle *node)
{
	int i;

	i = 0;
	
	char *para;
	para = "False";
	if(node->is_in_para)
		para = "True";
	printf("Content: %s // Token: %s // CC: %i // In Para: %s //", node->content, tokenToString(node->token), node->condition_count, para);
	if(node->args)
	{
		while (node->args[i])
		{
			printf("Arg %i: %s // ", i, node->args[i]);
			i++;
		}
		printf("\n");
	}
	else
		printf(" No args\n");
}

void ft_lstiter_middle(t_middle *first)
{
	while(first)
	{
		print_middle(first);
		first = first->next;
	}
}