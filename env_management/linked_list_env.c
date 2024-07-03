#include "../minishell.h"

void set_joinables(char *str, bool *before_joinable, bool *after_joinable)
{
	int i;

	i = 0;
	*before_joinable = true;
	*after_joinable = true;
	while(str[i] && str[i] != '=')
		i++;
	i++;
	if(str[i] == ' ')
		*before_joinable = false;
	while(str[i])
		i++;
	i--;
	if(str[i] == ' ')
		*after_joinable = false;
}


t_env	*ft_lstnew_env(char *env, int mode)
{
	t_env	*new_node;

	if(mode == MANUAL)
	{
		new_node = malloc(sizeof(t_env));
		if(!new_node)
			return (NULL);
		new_node->value = ft_strdup(env, MANUAL);
		if(!new_node->value)
			return (free(new_node), NULL);
	}
	else
	{
		new_node = smart_malloc(sizeof(t_env));
		new_node->value = ft_strdup(env, GC);
	}
	set_joinables(env, &new_node->before_joinable, &new_node->after_joinable);
	new_node->star_to_replace = REPLACE_ALL;
	new_node->envyable = true;
	new_node->next = NULL;
	new_node->prev = NULL;
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
		*lst = new;
		return ;
	}
	last_env = ft_lstlast_env(*lst);
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

char *new_shlvl(char *new_lvl)
{
	char *new_shlvl;
	char *lvl;

	lvl = ft_itoa(ft_atoi(new_lvl + 6) + 1, MANUAL);
	null_protector(lvl);
	new_shlvl = ft_strjoin("SHLVL=", lvl, MANUAL);
	null_protector(new_shlvl);
	free(lvl);
	return (new_shlvl);
}


t_env *array_to_env(char **env)
{
    t_env *current;
    t_env *head;
    int i;
	bool set_shlvl;
    char *new;

    i = 0;
    head = NULL;
	set_shlvl = false;
    while (env[i])
    {
        current = ft_lstnew_env(env[i], MANUAL);
		store_malloced(current->value);
		store_malloced(current);
        if (!ft_strncmp(current->value, "SHLVL", 5) && current->value[5] == '=')
        {
			set_shlvl = true;
            new = new_shlvl(current->value);
			store_malloced(new);
            free(current->value); // REMOVE FROM STORED
			remove_ptr(current->value);
            current->value = new;
        }
        ft_lstadd_back_env(&head, current);
        i++;
    }
	if(!set_shlvl)
		export_core(&head, "SHLVL=1");
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
	int i;

	i = 0;
	arr = smart_malloc((ft_lstsize_env(env)) * sizeof(char *));
	while(env)
	{
		if((env->value[0] != '?' || env->value[1] != '=') && env->envyable)
		{
			arr[i] = ft_strdup(env->value, GC);
			i++;
		}
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}

t_env *copy_env(t_env *env)
{
	t_env *head;
	t_env *current;

	head = NULL;
	while(env)
	{
		current = ft_lstnew_env(env->value, GC);
		ft_lstadd_back_env(&head, current);
		env = env->next;
	}
	return (head);
}

void ft_lstiter_env(t_env *env)
{
    int i;

    while (env)
    {
        i++;
        if (ft_strncmp(env->value, "?=", 2) && env->envyable)
			printf("%s\n", env->value);
        env = env->next;
    }
}


void ft_lstclear_env(t_env *head)
{
    t_env *temp;

    while (head)
    {
        temp = head;
        head = head->next;
        if (temp->value)
        {
            free(temp->value);
            temp->value = NULL;
        }
        free(temp);
        temp = NULL;
    }
}
