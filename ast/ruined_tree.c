#include "../minishell.h"


void print_ascii_tree(t_treenode *root, int level)
{
	t_middle *lol;
    if (root == NULL)
	{
		printf("\n");
        return;
	}

    // Print the right child
    print_ascii_tree(root->right, level + 1);

    // Print the current node
    for (int i = 0; i < level; i++)
        printf("    ");
    // printf("%s\n", root->content);
	printf("%s", root->content);
	int j = 0;
	if(root->args)
	{
		while (root->args[j])
		{
			printf(" %s", root->args[j]);
			j++;
		}
	}
    // Print the left child
    print_ascii_tree(root->left, level + 1);
}
 // TO INCLUDE IN HEADER
t_treenode *parse_cmdline(t_middle **middled);
t_treenode *parse_cmdlist(t_middle **middled)
{
	t_middle *cmd;
	t_treenode *result;

	if((*middled)->token == OPEN_PARANTHESE)
	{
		(*middled) = (*middled)->next;
		result = parse_cmdline(middled);
	}
	else
	{
		result = new_treenode(*middled);
		(*middled) = (*middled)->next;
	}
	return (result);
}

// <pipeline>  ::= <command> {"|" <command>}
t_treenode *parse_pipeline(t_middle **middled)
{
	t_treenode *l_node;
	t_treenode *r_node;
	t_treenode *pipe;

	pipe = NULL;
	l_node = parse_cmdlist(middled);
	while((*middled) && (*middled)->token == PIPE_LINE)
	{
		pipe = new_treenode(*middled);
		(*middled) = (*middled)->next;
		r_node = parse_pipeline(middled);
		pipe->left = l_node;
		pipe->right = r_node;
	}
	if(pipe)
		return (pipe);
	return (l_node);
}

// <cmdline>  ::= <pipeline> {("&&" | "||") <pipeline>
t_treenode *parse_cmdline(t_middle **middled)
{
	t_treenode *l_node;
	t_treenode *op;
	t_treenode *r_node;

	op = NULL;
	l_node = parse_pipeline(middled);
	while((*middled) && ((*middled)->token == AND || (*middled)->token == OR))
	{
		op = new_treenode(*middled);
		(*middled) = (*middled)->next;
		r_node = parse_pipeline(middled);
		op->left = l_node;
		op->right = r_node;
		l_node = op;
	}
	if((*middled) && (*middled)->token == CLOSE_PARANTHESE)
		(*middled) = (*middled)->next;
	if(op)
		return (op);
	return (l_node);
}



t_treenode *ruined_tree(t_middle *middled)
{
	t_treenode *root;

	root = parse_cmdline(&middled);
	print_ascii_tree(root, 0);
	return (root);
}
