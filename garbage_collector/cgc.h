#ifndef CGC_H
#define CGC_H

#include <stdlib.h>

#define SET 0
#define GET 1

#define ERROR 0
#define SUCCESS 1

typedef struct s_node {
    void *ptr_to_free;
    struct s_node* next;
} t_node;

// Smart allocators
void *smart_malloc(size_t size);
void smart_free(void);

// Linked list utils
t_node	*ft_lstnew(void *ptr_to_store);
void	ft_lstadd_back(t_node *lst, t_node *new_node);
void ft_lstclear(t_node **lst);

#endif