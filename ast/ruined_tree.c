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
    // Print the left child
    print_ascii_tree(root->left, level + 1);
}
// SINCE I DON'T PLAN ON HANDLING REDIRECTIONS OR NONE OF THAT RN IT WILL JUST RETURN A NODE AND MOVE THE LINKED LIST
t_treenode *parse_cmdlist(t_middle **middled)
{
	t_middle *cmd;

	cmd = *middled;
	(*middled) = (*middled)->next;
	return (new_treenode(cmd));
}

// <pipeline>  ::= <command> {"|" <command>}
t_treenode *parse_pipeline(t_middle **middled)
{
	t_treenode *l_node;
	t_treenode *r_node;
	t_treenode *pipe;

	l_node = parse_cmdlist(middled);
	if((*middled) && (*middled)->token == PIPE_LINE)
	{
		pipe = new_treenode(*middled);
		(*middled) = (*middled)->next;
		r_node = new_treenode(*middled);
		(*middled) = (*middled)->next;
		pipe->left = l_node;
		pipe->right = r_node;
		return (pipe);
	}
	(*middled) = (*middled)->next;
	return (l_node);
}

// <cmdline>  ::= <pipeline> {("&&" | "||") <pipeline>
t_treenode *parse_cmdline(t_middle **middled)
{
	t_treenode *l_node;
	t_treenode *op;
	t_treenode *r_node;

	l_node = parse_pipeline(middled);
	if((*middled)->token == AND || (*middled)->token == OR)
	{
		op = new_treenode(*middled);
		(*middled) = (*middled)->next;
		r_node = parse_pipeline(middled);
		(*middled) = (*middled)->next;
		op->left = l_node;
		op->right = r_node;
		return (op);
	}
	(*middled) = (*middled)->next;
	return (l_node);
}



t_treenode *ruined_tree(t_middle *middled)
{
	t_treenode *root;

	root = parse_cmdline(&middled);
	print_ascii_tree(root, 0);
	return (root);
}
