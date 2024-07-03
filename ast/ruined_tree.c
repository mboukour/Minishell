#include "../minishell.h"

// DEBUG MODE:
void print_ascii_tree(t_treenode *root, int level)
{
	t_arg *args;
	t_cmd_arg *cmd_arg;
	t_arg *command;
	if (root == NULL)
	{
		printf("\n");
		return;
	}
	print_ascii_tree(root->right, level + 1);
	for (int i = 0; i < level; i++)
        printf("    ");
	if(root->before_redir)
		ft_lstiter_redir(root->before_redir);
	command = root->command;
	if(!command)
		printf("None");
	else
	{
		while (command)
		{
			printf("%s:%i:%s", command->content, command->to_replace, tokenToString(command->token));
			command = command->next;
		}
	}
	// printf("%s:%i", root->content, root->to_replace);

	printf(" ");
	if(root->cmd_arg)
	{
		cmd_arg = root->cmd_arg;
		while (cmd_arg)
		{
			args = cmd_arg->arg;
			while(args)
			{
				printf("%s:%i:%s-", args->content, args->to_replace, tokenToString(args->token));
				args = args->next;
			}
			printf(" // ");
			cmd_arg = cmd_arg->next;
		}
	}
	else
		printf("No args");
	printf(" ");
	if(root->after_redir)
		ft_lstiter_redir(root->after_redir);
    print_ascii_tree(root->left, level + 1);
}

bool is_redir_mid(t_middle *middled)
{
	return (middled->token == REDIR_OUT || middled->token == DREDIR_OUT || middled->token == HERE_DOC || middled->token == REDIR_IN);
}

t_redir *handle_after_redirs(t_middle **middled)
{
	t_redir *result;
	t_redir *current;

	result = NULL;
	while(*middled && is_redir_mid(*middled))
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
	t_treenode *l_node;
	bool command_flag;

	command_flag = false;
	if((*middled)->token == OPEN_PARANTHESE)
	{
		(*middled) = (*middled)->next;
		l_node = parse_cmdline(middled);
		return (l_node);
	}
	before_redir = handle_before_redirs(middled);
	if((*middled) && ((*middled)->token != REDIR_OUT && (*middled)->token != DREDIR_OUT) && (*middled)->token != AND && (*middled)->token != OR && (*middled)->token != PIPE_LINE)
	{
		command_flag = true;
		l_node = new_treenode(*middled);  // ls
	}
	else
		l_node = new_treenode(NULL);
	l_node->before_redir = before_redir;
	if((*middled))
	{
		if(command_flag)
			(*middled) = (*middled)->next;
		l_node->after_redir = handle_after_redirs(middled);
	}
	return (l_node);
}

// <pipeline>  ::= <command> {"|" <command>}
t_treenode *parse_pipeline(t_middle **middled)
{
	t_treenode *l_node;
	t_treenode *r_node;
	t_treenode *pipe;

	pipe = NULL;
	l_node = parse_command(middled); // ls | (cat || wc)
	while((*middled) && (*middled)->token == PIPE_LINE)
	{
		pipe = new_treenode(*middled); // pipe
		(*middled) = (*middled)->next;
		r_node = parse_pipeline(middled); // ls
		pipe->left = l_node;
		pipe->right = r_node;
	}
	if(pipe)
		return (pipe);
	return (l_node); // ls
}

// <cmdline>  ::= <pipeline> {("&&" | "||") <pipeline>
t_treenode *parse_cmdline(t_middle **middled)
{
	t_treenode *l_node;
	t_treenode *op;
	t_treenode *r_node;

	op = NULL;
	l_node = parse_pipeline(middled); // ls
	while((*middled) && ((*middled)->token == AND || (*middled)->token == OR))
	{
		op = new_treenode(*middled); // AND OR
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
	return (root);
}
