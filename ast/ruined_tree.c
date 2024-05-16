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
	lol = root->first;
	if(root->is_terminal)
		printf("%s ==> Terminal // Root is: %p", root->first->content, root);
	else
	{
		while(lol != root->last)
		{
			printf("%s ", lol->content);
			lol = lol->next;
		}
		printf("%s ==> Non-terminal // Root is: %p", lol->content, root);

	}

    // Print the left child
    print_ascii_tree(root->left, level + 1);
}


// void print_conditions(t_middle *middle)
// {
// 	int i;

// 	i = 0;
// 	while(middle)
// 	{
// 		if(middle->token == COMMAND && !middle->is_in_para)
// 			printf("COMMAND %i: %s\n", middle->content);
// 		else if(middle->token)
// 	}
// }


t_treenode *make_mini(t_middle *middle)
{
	t_treenode *root;
	bool in_para;


}

void atomize_para(t_middle *open, t_middle *close, int cc)
{
	while(open != close->next)
	{
		open->is_in_para = true;
		open->condition_count = cc;
		open = open->next;
	}
}

void set_condition_count(t_middle *first, t_middle *last)
{
	int condition_count;
	t_middle *pre_last;
	int nesting_level;

	condition_count = 0;
	nesting_level = 0;
	while(first && first != last)
	{
		first->condition_count = condition_count;
		if (first->token == OPEN_PARANTHESE)
		{
			pre_last = ft_lstlast_middle(first);
			while(pre_last->token != CLOSE_PARANTHESE)
				pre_last = pre_last->prev;
			atomize_para(first, pre_last, condition_count);
			first = pre_last->next;
		}
		else 
		{
			if(first->token == AND || first->token == OR)
				condition_count++;
			first = first->next;
		}

	}
}

void handle_expression(t_middle *first, t_middle *last, t_treenode **root)
{
	while(first && first != last) // Protection, will debug later ig
	{
		if (first->token == AND || first->token == OR)
		{

		}
	}
}

void ruined_tree(t_middle *first, t_middle *last, t_treenode **root)
{

	if(!*root) // FIRST CALL ==> ROOT IS NULL
		*root = malloc(sizeof(t_treenode));
	set_condition_count(first, last);
	
}
