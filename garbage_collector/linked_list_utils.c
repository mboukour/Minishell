#include "cgc.h"

t_node	*ft_lstnew(void *ptr_to_store)
{
	t_node	*new_node;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
		return (NULL);
	new_node->ptr_to_free = ptr_to_store;
	new_node->next = NULL;
	return (new_node);
}
t_node	*ft_lstlast(t_node *lst)
{
	t_node	*temp;

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

void	ft_lstadd_back(t_node *lst, t_node *new_node)
{
	if (!lst)
		return ;
	ft_lstlast(lst)->next = new_node;
}

void ft_lstclear(t_node **lst)
{
    t_node *tmp;

    if (lst == NULL)
        return;
    
    while (*lst != NULL)
    {
        tmp = (*lst)->next;
        free((*lst)->ptr_to_free);
        free(*lst);
        *lst = tmp;
    }
}
