#include "../minishell.h"


t_cmd_arg *ft_lstnew_cmd_arg(t_arg *arg)
{
	t_cmd_arg *new_node;

	new_node = smart_malloc(sizeof(t_cmd_arg));
	new_node->arg = arg;
	// if(!arg)
	// 	fprintf(stderr, "Received NULL\n");
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

t_cmd_arg	*ft_lstlast_cmd_arg(t_cmd_arg *lst)
{
	t_cmd_arg	*temp;

	temp = lst;
	while (temp)
	{
		if (!temp->next)
			return (temp);
		temp = temp->next;
	}
	return (lst);
}

void ft_lstaddback_cmd_arg(t_cmd_arg **lst, t_cmd_arg *new)
{
	t_cmd_arg	*last_cmd_arg;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last_cmd_arg = ft_lstlast_cmd_arg(*lst);
	last_cmd_arg->next = new;
	new->prev = last_cmd_arg;
}


void ft_lstiter_cmd_arg(t_cmd_arg *cmd_arg)
{
	t_arg *arg;

	printf("Start\n");
	while(cmd_arg)
	{
		arg = cmd_arg->arg;
		while(arg)
		{
			printf("%s //");
			arg = arg->next;
		}
		cmd_arg = cmd_arg->next;
		printf("\n");
	}
	printf("End\n");

}