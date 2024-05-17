#include "../minishell.h"


void print_ascii_tree(t_treenode *root, int level)
{
	t_middle *lol;
    if (root == NULL)
	{
		printf("\n");
        return;
	}
    print_ascii_tree(root->right, level + 1);
    for (int i = 0; i < level; i++)
        printf("    ");
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
    print_ascii_tree(root->left, level + 1);
}
t_treenode *parse_cmdline(t_middle **middled);  // TO INCLUDE IN HEADER

bool is_redir(t_middle *middle)
{
	return(middle->token == REDIR_IN || middle->token == REDIR_OUT || middle->token == DREDIR_OUT || middle->token == HERE_DOC);
}

// t_treenode *parse_redir(t_middle **middled)
// {
// 	t_treenode *result;

// 	while(is_redir(*middled))
// 	{

// 	}
// }

// t_treenode *parse_cmdlist(t_middle **middled)
// {

// 	if((*middled)->token == WORD)
// 	{
		
// 	}
// }

// <cmdlist>  | "(" <cmdline> ")" <redir>
t_treenode *parse_command(t_middle **middled)
{
	t_middle *cmd;
	t_treenode *l_node;

	if((*middled)->token == OPEN_PARANTHESE)
	{
		(*middled) = (*middled)->next;
		return (parse_cmdline(middled));
	}
	l_node = new_treenode(*middled);
	(*middled) = (*middled)->next;
	return (l_node);
}

// <pipeline>  ::= <command> {"|" <command>}
t_treenode *parse_pipeline(t_middle **middled)
{
	t_treenode *l_node;
	t_treenode *r_node;
	t_treenode *pipe;

	pipe = NULL;
	l_node = parse_command(middled);
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
