#include "cgc.h"

void *globalizer_fd(int mode, void *ptr)
{
    static t_fd *head;

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


int store_fds(int fd) 
{
    t_fd *head;
    t_fd *new_node;

	if(fd == -1)
		return (ERROR);
    head = globalizer_fd(GET, NULL);
    new_node = ft_lstnew_fd(fd); 
    if (!head)
    {
        globalizer_fd(SET, new_node);
        return (SUCCESS);
    }
    else
    {
        ft_lstadd_back_fd(head, new_node);
        return (SUCCESS);
    }
}


void smart_close()
{
	t_fd *fd_list;

	fd_list = globalizer_fd(GET, NULL);
	while(fd_list)
	{
		close(fd_list->fd);
		fd_list = fd_list->next;
	}
}


void remove_fd_node(int fd_to_remove)
{
	t_fd *head;

	head = globalizer_fd(GET, NULL);
	if(!head)
		return;
	while(head)
	{
		if(head->fd == fd_to_remove)
		{
			if(head->prev)
				head->prev->next = head->next;
			else
			{
				if(head->next)
					head->next->prev = NULL;
				globalizer_fd(SET, head->next);
			}
			if(head->next)
				head->next->prev = head->prev;
			break;
		}
		head = head->next;
	}
}