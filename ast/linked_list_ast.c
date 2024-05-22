#include "../minishell.h"


char **copy_args(char **args, char *cmd)
{
	char **result;
	int c;

	c = 0;
	while(args[c])
		c++;
	result = smart_malloc((c + 2) * sizeof(char *));
	c = 0;
	result[0] = cmd;
	while(args[c])
	{
		result[c + 1] = ft_strdup(args[c]);
		c++;
	}
	result[c + 1] = NULL;
	return (result);
}

void nullify_all(t_treenode *node)
{
	node->token = NONE;
	node->content = NULL;
	node->args = NULL;
}

t_treenode		*new_treenode(t_middle *middled)
{
	t_treenode		*new_node;

	new_node = smart_malloc(sizeof(t_treenode));
	if(middled)
	{
		new_node->token = middled->token;
		new_node->content = ft_strdup(middled->content);
		new_node->args = NULL;
		if(middled->args)
			new_node->args = copy_args(middled->args, new_node->content);
		else
		{
			new_node->args = smart_malloc(2 * sizeof(char *));
			new_node->args[0] = middled->content;
			new_node->args[1] = NULL;
		}
	}
	else
		nullify_all(new_node);
	new_node->after_redir = NULL;
	new_node->before_redir = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}


t_redir *ft_lstnew_redir(t_middle *middled)
{
	t_redir *new_node;

	new_node = smart_malloc(sizeof(t_redir));
	if(!new_node)
		return (NULL);
	new_node->token = (middled)->token;
	new_node->redir_string = ft_strdup((middled)->redir_string);
	new_node->next = NULL;
	return (new_node);
}

t_redir *ft_lstlast_redir(t_redir *first)
{
	while(first)
	{
		if(!first->next)
			return (first);
		first = first->next;
	}
	return (NULL);
}

void ft_lstaddback_redir(t_redir **head, t_redir *new)
{
	t_redir *last;
	if(!head)
		return;
	if(!*head)
	{
		*head = new;
		return ;
	}
	last = ft_lstlast_redir(*head);
	last->next = new;
}

void ft_lstiter_redir(t_redir *first)
{
	while(first)
	{
		printf("(%s %s) ", tokenToString(first->token), first->redir_string);
		first = first->next;
	}
}

