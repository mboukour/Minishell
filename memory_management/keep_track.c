#include "../minishell.h"


void error_procedure()
{
    rl_clear_history();
	free_program();
	smart_free();
	ft_putstr_fd(2, FAILURE_MSG);
	exit(EXIT_FAILURE);
}

void *globalizer_manual(int mode, void *ptr)
{
    static t_node *head;

    if(mode == SET)
    {
        head = ptr;
        return (NULL);
    }
    else if (mode == GET)
        return (head);
    else // SHOULD NEVER TRIGGER, IMPLEMENTED TO BYPASS THE WARNING. (RESPECT MODES)
        return (NULL); 
}

void null_protector(void *to_check)
{
	if(!to_check)
		error_procedure();
}

void store_malloced(void *ptr_to_add) 
{
    t_node *head;
    t_node *new_node;

	null_protector(ptr_to_add);
    head = globalizer_manual(GET, NULL);
    new_node = ft_lstnew(ptr_to_add);
	null_protector(new_node);
    if (!head)
        globalizer_manual(SET, new_node);
	else
        ft_lstadd_back(head, new_node);
}



void free_program()
{
    t_node *head;

    head = globalizer_manual(GET, NULL);
    ft_lstclear(&head);
    globalizer_manual(SET, NULL);
}


void remove_ptr(void *ptr_to_rm)
{
	t_node *head;

	head = globalizer_manual(GET, NULL);
	if(!head)
		return;
	while(head)
	{
		if(head->ptr_to_free == ptr_to_rm)
		{
			if(head->prev)
				head->prev->next = head->next;
			else
			{
				if(head->next)
					head->next->prev = NULL;
				globalizer_manual(SET, head->next);
			}
			if(head->next)
				head->next->prev = head->prev;
			free(head);
			break;
		}
		head = head->next;
	}
}