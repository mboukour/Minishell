#include "../minishell.h"



void swap(t_env *a, t_env *b)
{
    char *temp = a->value;
    a->value = b->value;
    b->value = temp;
}

void sort_env_list(t_env *head)
{
    int swapped;
    t_env *ptr1;
    t_env *lptr = NULL;

    swapped = 1;
    while (swapped)
    {
        swapped = 0;
        ptr1 = head;

        while (ptr1 && ptr1->next != lptr)
        {
            if (ft_strcmp(ptr1->value, ptr1->next->value) > 0) 
            {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
}