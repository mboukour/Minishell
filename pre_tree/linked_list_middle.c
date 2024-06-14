#include "../minishell.h"


int assing_builtin(char *commad) 
{
    if (!ft_strcmp(commad, "echo"))
        return (ECHO);
    else if (!ft_strcmp(commad, "exit"))
        return (EXIT);
    else if (!ft_strcmp(commad, "cd"))
        return (CD);
    else if (!ft_strcmp(commad, "pwd"))
        return (PWD);
    else if (!ft_strcmp(commad, "export"))
        return (EXPORT);
    else if (!ft_strcmp(commad, "unset"))
        return (UNSET);
    else if (!ft_strcmp(commad, "env"))
        return (ENV_CMD);
    return (NONE);
}

t_middle	*ft_lstnew_middle(t_arg *command, t_cmd_arg *cmd_arg, int token)
{
	t_middle	*new_node;

	new_node = smart_malloc(sizeof(t_middle));
	if (!new_node)
		return (NULL);
	new_node->token = token;
	// new_node->content = content;
	new_node->command = command;
	new_node->args = NULL;
	new_node->cmd_arg = cmd_arg;
	new_node->redir_string = NULL;
	new_node->to_replace = REPLACE_ALL;
	// new_node->builtin = assing_builtin(content);
	new_node->builtin = NONE;
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
	t_arg *command;
	t_cmd_arg *loop;
	t_arg *mini_args;

	i = 0;
	command = node->command;

	if(!command)
		printf("None-");
	while(command)
	{
		printf("%s:%i-", command->content, command->to_replace);
		command = command->next;
	}
	printf(" // Token: %s //", tokenToString(node->token));
	if(node->cmd_arg)
	{
		loop = node->cmd_arg;
		while(loop)
		{
			mini_args = loop->arg;
			while(mini_args)
			{
				printf("%s:%i-", mini_args->content, mini_args->to_replace);
				mini_args = mini_args->next;
			}
			loop = loop->next;
		}
	}
	if(node->token == REDIR_IN || node->token == REDIR_OUT || node->token == DREDIR_OUT || node->token == HERE_DOC)
	{
		command = node->redirections;
		if(node->token == REDIR_IN)
			printf("Read from: // ");
		else if (node->token == REDIR_OUT || node->token == DREDIR_OUT)
			printf("Write to: // ");
		else if (node->token == HERE_DOC)
			printf("Delimiter: // ");
		while(command)
		{
			printf("%s:%i //", command->content, command->to_replace);
			command = command->next;
		}
	}
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