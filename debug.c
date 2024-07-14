#include "minishell.h"
char *redir_to_str(int token)
{
	if(token == DREDIR_OUT)
		return (">>");
	else if(token == REDIR_OUT)
		return (">");
	else if (token == REDIR_IN)
		return ("<");
	else
		return ("<<");
}

void ft_lstiter_redir(t_redir *first)
{
	t_arg *arg;
	while(first)
	{
		arg = first->redir_input;
		printf("(%s: ", redir_to_str(first->token));
		while(arg)
		{
			printf("%s:%i //", arg->content, arg->to_replace);
			arg = arg->next;
		}
		printf(")");
		first = first->next;
	}
}


const char* tokenToString(int t)
{
    switch (t) 
	{
        case COMMAND: return "COMMAND";
        case WHITE_SPACE: return "WHITE_SPACE";
        case NEW_LINE: return "NEW_LINE";
        case QUOTE: return "QUOTE";
        case DOUBLE_QUOTE: return "DOUBLE_QUOTE";
        case ENV: return "ENV";
        case PIPE_LINE: return "PIPE_LINE";
        case REDIR_IN: return "REDIR_IN";
        case REDIR_OUT: return "REDIR_OUT";
        case HERE_DOC: return "HERE_DOC";
        case DREDIR_OUT: return "DREDIR_OUT";
		case STAR: return "STAR";
		case AND: return "AND";
		case OR: return "OR";
		case OPEN_PARANTHESE: return "OPEN_PARA";
		case ENV_STAR: return "ENV_STAR";
		case CLOSE_PARANTHESE: return "CLOSE_PARA";
		case NONE: return "NONE";
        default: return ft_strjoin("UNKOWN: ", ft_itoa(t, GC), GC);
    }
}

const char* stateToString(enum e_state s)
{
    switch (s) 
	{
        case IN_DQUOTE: return "IN_DQUOTE";
        case IN_QUOTE: return "IN_QUOTE";
		case IN_PARENTHESES: return "IN_PARENTHESES";
        case GENERAL: return "GENERAL";
        default: return "UNKNOWN_STATE";
    }
}


void print_lex(const t_lex *lex) {
	// char *c = "False";
	// if(lex->to_replace)
	// 	c = "True";
    // printf("| %-10s | %-15s | %-10d |\n", lex->content, tokenToString(lex->token), lex->len);
    // printf("| %-10s | %-15s | %-10s |\n", "", "", "");
    // printf("|------------|-----------------|------------|\n");
	printf("Content: {%s} // Token: %s // To replace: %i // JC: %i\n", lex->content, tokenToString(lex->token), lex->to_replace, lex->join_count);
}

void ft_lstiter_lex(t_lex *lex)
{
    // printf("| %-10s | %-15s | %-10s | \n", "Content", "Token", "Length");
    // printf("|------------|-----------------|------------|\n");
	while(lex)
	{
		print_lex(lex);
		lex = lex->next;
	}
}
void print_middle(t_middle *node)
{
	t_arg *command;
	t_cmd_arg *loop;
	t_arg *mini_args;

	command = node->command;

	if(!command)
		printf("None-");
	while(command)
	{
		printf("%s:%i-", command->content, command->to_replace);
		command = command->next;
	}
	printf(" // Token: %s //", tokenToString(node->token));
	if(node->cmd_arg)
	{
		loop = node->cmd_arg;
		while(loop)
		{
			mini_args = loop->arg;
			while(mini_args)
			{
				printf("%s:%i-", mini_args->content, mini_args->to_replace);
				mini_args = mini_args->next;
			}
			loop = loop->next;
		}
	}
	if(node->token == REDIR_IN || node->token == REDIR_OUT || node->token == DREDIR_OUT || node->token == HERE_DOC)
	{
		command = node->redirections;
		if(node->token == REDIR_IN)
			printf("Read from: // ");
		else if (node->token == REDIR_OUT || node->token == DREDIR_OUT)
			printf("Write to: // ");
		else if (node->token == HERE_DOC)
			printf("Delimiter: // ");
		while(command)
		{
			printf("%s:%i //", command->content, command->to_replace);
			command = command->next;
		}
	}
	printf("\n");
}

void ft_lstiter_middle(t_middle *first)
{
	while(first)
	{
		print_middle(first);
		first = first->next;
	}
}

void ft_lstiter_cmd_arg(t_cmd_arg *cmd_arg)
{
	t_arg *arg;

	printf("---------------------\n");
	while(cmd_arg)
	{
		arg = cmd_arg->arg;
		while(arg)
		{
			printf("%s //", arg->content);
			arg = arg->next;
		}
		cmd_arg = cmd_arg->next;
		printf("\n");
	}
	printf("---------------------\n");
}

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