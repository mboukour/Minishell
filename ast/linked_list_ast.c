#include "../minishell.h"


char **copy_args(char **args)
{
	char **result;
	int c;

	c = 0;
	while(args[c])
		c++;
	result = malloc((c + 1) * sizeof(char *));
	c = 0;
	while(args[c])
	{
		result[c] = ft_strdup(args[c]);
		c++;
	}
	result[c] = NULL;
	return (result);
}

t_treenode		*new_treenode(t_middle *middled)
{
	t_treenode		*new_node;

	new_node = malloc(sizeof(t_treenode));
	if (!new_node)
		return (NULL);
	// if(middled->token != COMMAND)
	// 	printf("NO COMMAND IN NEW: %s\n", middled->content);
	new_node->token = middled->token;
	new_node->content = ft_strdup(middled->content);
	// new_node->node = middled;
	new_node->args = NULL;
	if(middled->args)
		new_node->args = copy_args(middled->args);
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