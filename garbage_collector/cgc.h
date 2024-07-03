#ifndef CGC_H
#define CGC_H

#include <stdlib.h>

#define SET 0
#define GET 1

#define ERROR 0
#define SUCCESS 1

#define FAILURE_MSG "Error: Failed to allocate memory!\n"

typedef struct s_node 
{
    void *ptr_to_free;
    struct s_node* next;
	struct s_node *prev;
} t_node;

// Smart allocators
void *smart_malloc(size_t size);
void smart_free(void);
int store_mallocs(void *ptr_to_add);

// Save mallocated
void store_malloced(void *ptr_to_add);
void free_program();
void null_protector(void *to_check);
void remove_ptr(void *ptr_to_rm);

// Linked list utils
t_node	*ft_lstnew(void *ptr_to_store);
void	ft_lstadd_back(t_node *lst, t_node *new_node);
void ft_lstclear(t_node **lst);

#endif