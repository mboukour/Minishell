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
				middle->is_in_para = true;
				middle = middle->next;
			}
			middle->condition_count = condition_count;
		}
		middle = middle->next;
	}
}



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
		printf("%s", root->first->content);
	else
	{
		// printf("FISRT: %p // LAST: %p", lol, root->last);
		while(lol != root->last)
		{
			printf("%s ", lol->content);
			lol = lol->next;
		}
	}

    // Print the left child
    print_ascii_tree(root->left, level + 1);
}

t_treenode *setup_condition(t_middle *current_middle ,int mode)
{
	t_middle *king_node;
	t_middle *looping_node;
	int current_cc;
	int counter;

	counter = 0;
	current_cc = current_middle->condition_count;
	if(mode == LEFT)
	{
		king_node = current_middle->prev;
		looping_node = king_node;
		while(looping_node->condition_count == current_cc)
		{
			counter++;
			if(looping_node->prev)
				looping_node = looping_node->prev;
			else
				break;
		}
		if(counter == 1)
			return (ft_lstnew_treenode(king_node, NULL, true));
		return (ft_lstnew_treenode(looping_node, king_node, false));
	}
	else
	{
		king_node = current_middle->next;
		looping_node = king_node;
		while(looping_node->condition_count == current_cc + 1)
		{
			counter++;
			if(looping_node->next)
				looping_node = looping_node->next;
			else
				break;
		}

		if (counter == 1)
			return (ft_lstnew_treenode(king_node, NULL, true));
		return (ft_lstnew_treenode(king_node, looping_node, false));
	}
}

t_treenode *setup_mini_tree(t_middle *middle)
{
	t_treenode *current_node;
	t_treenode *current_root;

	current_node = NULL;
	current_root = NULL;
	while(middle)
	{
		if((middle->token == OR || middle->token == AND) && !middle->is_in_para)
		{
			current_node = ft_lstnew_treenode(middle, NULL ,true);
			if(!current_root)
			{
				current_root = current_node;
				current_root->left = setup_condition(middle, LEFT);
				current_root->right = setup_condition(middle, RIGHT);
			}
			else
			{
				change_root_to(&current_root, current_node);
				current_root->right = setup_condition(middle, RIGHT);
			}
		}
		middle = middle->next;
	}
	return (current_root);
}

t_treenode *rdp(t_middle *middle, t_treenode *current_root)
{
	t_treenode *root;

	set_condition_count(middle);
	root = setup_mini_tree(middle);
	print_ascii_tree(root, 0);
	return (NULL);
}