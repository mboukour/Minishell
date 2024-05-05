#include "../minishell.h"


void print_hold(t_lex *first, t_lex *current)
{
	if(first)
	{
		while(first && first != current)
		{
			printf("{%s} ", first->content);
			first = first->next;
		}
	}
	else
	{
		while(current)
		{
			printf("{%s} ",current->content);
			current = current->next;
		}
	}
}

bool is_thre_an_op(t_lex *current)
{
	while(current)
	{
		if(current->token == AND || current->token == OR)
			return (true);
		current = current->next;
	}
	return (false);
}

void set_condition_count(t_lex *lex)
{
	int condition_count;

	condition_count = 0;
	while(lex)
	{
		lex->condition_count = condition_count;
		if(lex->token == AND || lex->token == OR)
			condition_count++;
		else if (lex->token == OPEN_PARANTHESE)
		{
			while(lex->token != CLOSE_PARANTHESE)
			{
				lex->condition_count = condition_count;
				lex->is_a_para = true;
				lex = lex->next;
			}
			lex->condition_count = condition_count;
		}
		lex = lex->next;
	}
}

t_treenode *setup_condition(t_lex *lex, int current, int mode)
{
	t_lex *move;
	char *condition;
	int hold;

	condition = NULL;
	move = lex;
	if(mode == LEFT)
	{
		while (lex->condition_count != current)
			lex = lex->next;
		while(lex->prev && lex->condition_count == current)
			lex = lex->prev;
		while(lex && lex != move)
		{
			condition = ft_strjoin(condition, lex->content);
			lex = lex->next;
		}
	}
	else if(mode == RIGHT)
	{
		lex = lex->next;
		move = lex;
		hold = current - 1;
		while (move  && move->condition_count == current + 1)
			move = move->next;
		while(lex && lex != move)
		{
			condition = ft_strjoin(condition, lex->content);
			lex = lex->next;
		}
	}
	return (ft_lstnew_treenode(condition, CONDITION));
}


t_treenode *setup_mini_tree(t_lex *lex)
{
	t_treenode *current_node;
	t_treenode *current_root;
	t_lex *hold_current;

	current_root = NULL;
	while (lex)
	{
		if((lex->token == AND || lex->token == OR) && !lex->is_a_para)
		{
			current_node = ft_lstnew_treenode(ft_strdup(lex->content), lex->token);
			hold_current = lex;
			if(!current_root)
			{
				current_root = current_node;
				current_root->left =  setup_condition(lex, lex->condition_count, LEFT);
				current_root->right =  setup_condition(lex, lex->condition_count, RIGHT);
			}
			else
			{
				change_root_to(&current_root, current_node);
				current_root->right = setup_condition(lex, lex->condition_count, RIGHT);
			}
		}
		lex = lex->next;
	}
	return current_root;
}
void print_ascii_tree(t_treenode *root, int level) {
    if (root == NULL)
        return;

    // Print the right child
    print_ascii_tree(root->right, level + 1);

    // Print the current node
    for (int i = 0; i < level; i++)
        printf("    ");
    printf("%s\n", root->content);

    // Print the left child
    print_ascii_tree(root->left, level + 1);
}




t_treenode *rdp(t_lex *lex, t_treenode *current_root)
{
	t_treenode *root;
	set_condition_count(lex);
	root = setup_mini_tree(lex);
	print_ascii_tree(root, 0);
	return (NULL);
}