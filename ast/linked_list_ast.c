#include "../minishell.h"


t_treenode		*ft_lstnew_treenode(t_middle *first, t_middle *last, bool is_terminal)
{
	t_treenode		*new_node;

	new_node = malloc(sizeof(t_treenode));
	if (!new_node)
		return (NULL);
	// new_node->token = token;
	// new_node->content = content;
	new_node->first = first;
	new_node->last = last;
	new_node->is_terminal = is_terminal;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

void change_root_to(t_treenode **from_node, t_treenode *to_node)
{
	t_treenode *new_left;

	to_node->left = *from_node;
	*from_node = to_node;
}