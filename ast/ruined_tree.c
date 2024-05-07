#include "../minishell.h"



void set_condition_count(t_middle *middle)
{
	int condition_count;

	condition_count = 0;
	while(middle)
	{
		middle->condition_count = condition_count;
		if(middle->token == AND || middle->token == OR)
			condition_count++;
		else if (middle->token == OPEN_PARANTHESE)
		{
			while(middle->token != CLOSE_PARANTHESE)
			{
				middle->condition_count = condition_count;
				middle = middle->next;
			}
			middle->condition_count = condition_count;
		}
		middle = middle->next;
	}
}



void print_ascii_tree(t_treenode *root, int level)
{
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

// t_treenode *setu

t_treenode *rdp(t_middle *middle, t_treenode *current_root)
{
	t_treenode *root;

	set_condition_count(middle);
	// root = setup_mini_tree(lex);
	// print_ascii_tree(root, 0);
	return (NULL);
}