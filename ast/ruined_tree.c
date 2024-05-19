#include "../minishell.h"

// DEBUG MODE:
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
	// if(root->before_redir)
	// 	ft_lstiter_redir(root->before_redir);
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
	printf(" ");
	// if(root->after_redir)
	// 	ft_lstiter_redir(root->after_redir);
    print_ascii_tree(root->left, level + 1);
}

t_redir *handle_after_redirs(t_middle **middled)
{
	t_redir *result;
	t_redir *current;

	result = NULL;
	while(*middled && ((*middled)->token == REDIR_OUT || (*middled)->token == DREDIR_OUT))
	{
		current = ft_lstnew_redir(*middled);
		ft_lstaddback_redir(&result, current);
		(*middled) = (*middled)->next;
	}
	return (result);
}

t_redir *handle_before_redirs(t_middle **middled)
{
	t_redir *result;
	t_redir *current;

	result = NULL;
	while(*middled && ((*middled)->token == REDIR_IN || (*middled)->token == HERE_DOC))
	{
		current = ft_lstnew_redir(*middled);
		ft_lstaddback_redir(&result, current);
		(*middled) = (*middled)->next;
	}
	return (result);
}

t_treenode *parse_command(t_middle **middled)
{
	t_redir *before_redir;
	t_redir *after_redir;
	t_treenode *l_node;

	if((*middled)->token == OPEN_PARANTHESE)
	{
		(*middled) = (*middled)->next;
		l_node = parse_cmdline(middled);
		return (l_node);
	}
	before_redir = handle_before_redirs(middled);
	l_node = new_treenode(*middled);
	l_node->before_redir = before_redir;
	(*middled) = (*middled)->next;
	l_node->after_redir = handle_after_redirs(middled);
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
