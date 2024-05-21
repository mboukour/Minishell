#include "cgc.h"

void *globalizer_head(int mode, void *ptr)
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
int store_mallocs(void *ptr_to_add) 
{
    t_node *head;
    t_node *new_node;

    head = globalizer_head(GET, NULL);
    new_node = ft_lstnew(ptr_to_add);
    if (!new_node)
        return (ERROR);
    if (!head)
    {
        globalizer_head(SET, new_node);
        return (SUCCESS);
    }
    else
    {
        ft_lstadd_back(head, new_node);
        return (SUCCESS);
    }
}

void *smart_malloc(size_t size)
{
    void *ptr;
    t_node *head;

    head = globalizer_head(GET, NULL);
    ptr = malloc(size);
    if(ptr && store_mallocs(ptr))
        return (ptr);
    smart_free();
    return (NULL);
}

void smart_free() {
    t_node *head;

    head = globalizer_head(GET, NULL);
    ft_lstclear(&head);
    globalizer_head(SET, NULL);
}
