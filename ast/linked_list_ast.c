#include "../minishell.h"


t_treenode		*new_treenode(t_middle *middled)
{
	t_treenode		*new_node;

	new_node = malloc(sizeof(t_treenode));
	if (!new_node)
		return (NULL);
	// if(middled->token != COMMAND)
	// 	printf("NO COMMAND IN NEW: %s\n", middled->content);
	new_node->token = middled->token;
	// new_node->content = content;
	new_node->node = middled;
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