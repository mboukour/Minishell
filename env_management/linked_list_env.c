#include "../minishell.h"


t_env	*ft_lstnew_env(char *env)
{
	t_env	*new_node;
	char 	**args;

	new_node = malloc(sizeof(t_env));
	if(!new_node)
		return (NULL);
	new_node->value = ft_strdup(env, MANUAL);
	new_node->index = NONE;
	new_node->next = NULL;
	new_node->prev = NULL;
	free(args);
	return (new_node);
}

t_env	*ft_lstlast_env(t_env *lst)
{
	t_env	*temp;

	if (lst == NULL)
		return (NULL);
	temp = lst;
	while (temp)
	{
		if (!temp->next)
			return (temp);
		temp = temp->next;
	}
	return (lst);
}
void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*last_env;

	if (!lst)
		return ;
	if (!*lst)
	{
		new->index = 0;
		*lst = new;
		return ;
	}
	last_env = ft_lstlast_env(*lst);
	new->index = last_env->index + 1;
	last_env->next = new;	
	new->prev = last_env;
}

int ft_lstsize_env(t_env *env)
{
	int i;

	i = 0;
	while(env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

t_env *array_to_env(char **env)
{
	t_env *current;
	t_env *head;
	int i;

	i = 0;
	head = NULL;
	current = NULL;
	while(env[i])
	{
		current = ft_lstnew_env(env[i]);
		ft_lstadd_back_env(&head, current);
		i++;
	}
	return (head);
}

void free_until_k(char **args, int k)
{
	int i;

	i = 0;
	while(i < k)
	{
		free(args[i]);
		i++;
	}
	free(args);
}
char **env_to_array(t_env *env)
{
	char **arr;
	char *str;
	int i;

	i = 0;
	arr = malloc(ft_lstsize_env(env));
	if(!arr)
		return (NULL);
	while(env)
	{
		arr[i] = ft_strdup(env->value, MANUAL);
		if(!arr[i])
		{
			free_until_k(arr, i);
			return (NULL);
		}
		i++;
		env = env->next;
	}
	return (arr);
}

void ft_lstiter_env(t_env *env)
{
	while(env)
	{
		printf("%s\n", env->value);
		env = env->next;
	}
}

void ft_lstclear_env(t_env *env)
{
	t_env *hold;
	while(env)
	{
		hold = env->next;
		free(env->value);
		free(env);
		env = env->next;
	}
}