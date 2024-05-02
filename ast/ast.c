#include "../minishell.h"


t_treenode *make_tree(t_lex *lex_head)
{
	t_treenode *mini_root;
	t_treenode *mini_left;
	t_treenode *mini_right;
	t_treenode *root;

	root = NULL;
 	while(lex_head)
	{
		if (lex_head->token == OR || lex_head->token == AND)
		{
			mini_root = ft_lstnew_treenode(OPERATOR, NULL, lex_head->token);
			lex_head->added_to_ast = true;
		}
	}
}