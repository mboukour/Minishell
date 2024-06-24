#include "../minishell.h"

void nullify_all(t_treenode *node)
{
	node->token = NONE;
	node->content = NULL;
	node->args = NULL;
	node->command = NULL;
	node->cmd_arg = NULL;
	node->builtin = NONE;
}

t_arg *copy_arg(t_arg *arg)
{
	t_arg *res;
	t_arg *new;

	res = NULL;
	while(arg)
	{
		new = ft_lstnew_arg(NULL);
		new->content = ft_strdup(arg->content, GC);
		new->to_replace = arg->to_replace;
		new->token = arg->token;
		ft_lstaddback_arg(&res, new);
		arg = arg->next;
	}
	return (res);
}

t_treenode		*new_treenode(t_middle *middled)
{
	t_treenode		*new_node;

	new_node = smart_malloc(sizeof(t_treenode));
	if(middled)
	{
		new_node->token = middled->token;
		new_node->command = copy_arg(middled->command);
		new_node->builtin = middled->builtin;
		new_node->cmd_arg = middled->cmd_arg;
	}
	else
		nullify_all(new_node);
	new_node->content = NULL;
	new_node->args = NULL;
	new_node->after_redir = NULL;
	new_node->before_redir = NULL;
	new_node->is_a_directory = false;
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
	new_node->redir_string = NULL;
	new_node->redir_input = middled->redirections;
	new_node->to_replace = middled->to_replace;
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

char *redir_to_str(int token)
{
	if(token == DREDIR_OUT)
		return (">>");
	else if(token == REDIR_OUT)
		return (">");
	else if (token == REDIR_IN)
		return ("<");
	else
		return ("<<");
}

void ft_lstiter_redir(t_redir *first)
{
	t_arg *arg;
	while(first)
	{
		arg = first->redir_input;
		printf("(%s: ", redir_to_str(first->token));
		while(arg)
		{
			printf("%s:%i //", arg->content, arg->to_replace);
			arg = arg->next;
		}
		printf(")");
		first = first->next;
	}
}

