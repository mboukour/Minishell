/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:02:55 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/12 17:13:57 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGC_H
# define CGC_H

# include <stdlib.h>
# include <unistd.h>

# define SET 0
# define GET 1

# define ERROR 0
# define SUCCESS 1

# define FAILURE_MSG "Error: Failed to allocate memory!\n"

typedef struct s_node
{
	void			*ptr_to_free;
	struct s_node	*next;
	struct s_node	*prev;
}	t_node;

typedef struct s_fd
{
	int			fd;
	struct s_fd	*next;
	struct s_fd	*prev;
}	t_fd;

void	*smart_malloc(size_t size);
void	smart_free(void);
int		store_mallocs(void *ptr_to_add);
void	store_malloced(void *ptr_to_add);
void	free_program(void);
void	null_protector(void *to_check);
void	remove_ptr(void *ptr_to_rm);
void	error_procedure(void);
void	smart_close(void);
int		store_fds(int fd);
void	remove_fd_node(int fd_to_remove);
t_node	*ft_lstnew(void *ptr_to_store);
void	ft_lstadd_back(t_node *lst, t_node *new_node);
void	ft_lstclear(t_node **lst);
t_fd	*ft_lstnew_fd(int fd_to_store);
void	ft_lstadd_back_fd(t_fd *lst, t_fd *new_node);

#endif