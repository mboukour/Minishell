#include "../minishell.h"


t_treenode		*ft_lstnew_treenode(int type, char *expression, int operator)
{
	t_treenode		*new_node;

	new_node = malloc(sizeof(t_treenode));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->expression = expression;
	new_node->operator = operator;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

