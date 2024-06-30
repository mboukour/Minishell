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


t_env	*ft_lstnew_env(char *env)
{
	t_env	*new_node;
	bool before_joinable;

	new_node = malloc(sizeof(t_env));
	if(!new_node)
		return (NULL);
	new_node->value = ft_strdup(env, MANUAL);
	if(!new_node->value)
	{
		free(new_node);
		return (NULL);
	}
	set_joinables(env, &new_node->before_joinable, &new_node->after_joinable);
	new_node->star_to_replace = REPLACE_ALL;
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
	int i;
	char *new_shlvl;
	char *temp;
	char *lvl;

	i = 0;
	lvl = ft_itoa(ft_atoi(new_lvl + 6) + 1, MANUAL);
	if(!lvl)
		return (NULL);
	new_shlvl = ft_strjoin("SHLVL=", lvl, MANUAL);
	free(lvl);
	return (new_shlvl);
}


t_env *array_to_env(char **env)
{
    t_env *current;
    t_env *head;
    int i;
	bool set_shlvl;
	bool before_joinable;
	bool after_joinable;
    char *new;

    i = 0;
    head = NULL;
	set_shlvl = false;
	before_joinable = false;
	after_joinable = false;
    while (env[i])
    {
        current = ft_lstnew_env(env[i]);
        if (!current)
        {
            ft_lstclear_env(head);
            return (NULL);
        }
        if (!ft_strncmp(current->value, "SHLVL", 5) && current->value[5] == '=')
        {
			set_shlvl = true;
            new = new_shlvl(current->value);
            free(current->value);
            if (!new)
                return (free(current), ft_lstclear_env(head), NULL);
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
	char *str;
	int i;

	i = 0;
	arr = smart_malloc((ft_lstsize_env(env)) * sizeof(char *));
	while(env)
	{
		if((env->value[0] != '?' || env->value[1] != '='))
		{
			arr[i] = ft_strdup(env->value, MANUAL);
			if(!arr[i])
			{
				free_until_k(arr, i);
				return (NULL);
			}
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
		current = ft_lstnew_env(env->value);
		if(!current)
			return (ft_lstclear_env(head), NULL);
		ft_lstadd_back_env(&head, current);
		env = env->next;
	}
	return (head);
}

void ft_lstiter_env(t_env *env, bool add_declare)
{
    int i;
    t_env *copy;

    i = 0;
    if (add_declare)
    {
        copy = copy_env(env);
        if (!copy)
        {
            perror("Failed to copy environment");
            exit(EXIT_FAILURE);
        }
        sort_env_list(copy);
        env = copy;
    }

    while (env)
    {
        i++;
        if (ft_strncmp(env->value, "?=", 2))
        {
            if (add_declare)
			{
				if(ft_strncmp(env->value, "_=", 2))
					printf("declare -x %s\n", env->value);
			}
			else
				printf("%s\n", env->value);
        }
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
