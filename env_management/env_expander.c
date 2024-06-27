#include "../minishell.h"


int get_current_path(char *str)
{
	int i;
	
	i = 0;
	while(str[i] && str[i] != '$' && str[i] != '*')
		i++;
	return i;
}

t_lex	*ft_mini_lstnew_lex(char *content, int token, int len)
{
	t_lex	*new_node;

	new_node = smart_malloc(sizeof(t_lex));
	new_node->content = content;
	new_node->len = len;
	new_node->token = token;
	new_node->to_replace = REPLACE_ALL;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}



void mini_handle_word(char *input, int *i, t_lex **head)
{
	t_lex *current_node;
    char *content;
    int hold;

	hold = *i;
    while (input[*i] && input[*i] != '$' && input[*i] != '*' && input[*i] != ' ')
        (*i)++;
    content = ft_substr(input, hold, *i - hold, GC);
    current_node = ft_mini_lstnew_lex(content, WORD, *i - hold);
    ft_lstadd_back_lex(head, current_node);
}


void mini_handle_space(char *input, int *i, t_lex **head)
{
    int hold;
    char *content;
    t_lex *current_node;

	hold = *i;
    while (input[*i] == ' ')
        (*i)++;
    content = ft_substr(input, hold, *i - hold, GC);
    current_node = ft_mini_lstnew_lex(content, WHITE_SPACE, *i - hold);
    ft_lstadd_back_lex(head, current_node);
}


void mini_handle_star(char *input, int *i, t_lex **head)
{
	t_lex *current_node;
    int hold;
    int c;
    char *content;

    c = input[*i];
	hold = *i;
    (*i)++;
    while (input[*i] && input[*i] == '*' && input[*i] != '$' && input[*i] != ' ')
        (*i)++;
    content = ft_substr(input, hold, *i - hold, GC);
    current_node = ft_mini_lstnew_lex(content, c, *i - hold);
    ft_lstadd_back_lex(head, current_node);
}

void mini_handle_env(char *input, int *i, t_lex **head)
{
	t_lex *current_node;
    int hold;
    int c;
    char *content;
    bool star_bool;

    star_bool = false;
    c = input[*i];
	hold = *i;
    (*i)++;
    while (input[*i] && input[*i] != '$' && input[*i] != '*' && input[*i] != ' ')
        (*i)++;
    content = ft_substr(input, hold, *i - hold, GC);
    current_node = ft_mini_lstnew_lex(content, c, *i - hold);
    ft_lstadd_back_lex(head, current_node);
}


t_lex *mini_lexer(char *str)
{
	int i;
	t_lex *head;
	t_lex *current;

	i = 0;
	head = NULL;
	while(str[i])
	{
		if(str[i] == ' ')
			mini_handle_space(str, &i, &head);
		else if (str[i] == '*')
			mini_handle_star(str, &i, &head);
		else if (str[i] == '$')
			mini_handle_env(str, &i, &head);
		else
			mini_handle_word(str, &i, &head);
	}
	return (head);
}

t_arg *lex_to_arg(t_lex *lex)
{
	t_arg *head;
	t_arg *current;

	head = NULL;
	while(lex)
	{
		current = ft_lstnew_arg(lex);
		ft_lstaddback_arg(&head, current);
		lex = lex->next;
	}
	return (head);
}

char *env_expander(char *to_expand, t_env *env)
{
	t_lex *mini_lexed;
	t_lex *original;
	t_env *env_node;
	t_arg *result;
	char *me;

	mini_lexed = mini_lexer(to_expand);
	original = mini_lexed;
	while(mini_lexed)
	{ 
		if(mini_lexed->token == ENV)
		{
			env_node = get_env(env, mini_lexed->content + 1);
			if(!env_node)
				mini_lexed->content = ft_strdup("", GC);
			else
			{
				me = env_node->value;
				while(*me && *me != '=')
					me++;
				me++;
				mini_lexed->content = ft_strdup(me, GC);
			}
		}
		mini_lexed = mini_lexed->next;
	}
	result = lex_to_arg(original); // RETURN THIS
	return (me);
}

void remove_arg_node(t_arg **head_ref, t_arg *node_to_remove)
{
    if (*head_ref == NULL || node_to_remove == NULL)
        return;
    if (*head_ref == node_to_remove)
        *head_ref = node_to_remove->next;
    if (node_to_remove->next != NULL)
        node_to_remove->next->prev = node_to_remove->prev;
    if (node_to_remove->prev != NULL)
        node_to_remove->prev->next = node_to_remove->next;
    node_to_remove->next = NULL;
    node_to_remove->prev = NULL;
}

void replace_node_with_list(t_arg **head_ref, t_arg *node_to_replace, t_arg *new_list_head)
{
	t_arg *new_list_tail;

    if (*head_ref == NULL || node_to_replace == NULL || new_list_head == NULL)
        return;
	
    if (*head_ref == node_to_replace)
        *head_ref = new_list_head;
    if (node_to_replace->prev != NULL)
    {
        node_to_replace->prev->next = new_list_head;
        new_list_head->prev = node_to_replace->prev;
    }
    else
        new_list_head->prev = NULL;
	new_list_tail = ft_lstlast_arg(new_list_head);
    if (node_to_replace->next != NULL)
    {
        node_to_replace->next->prev = new_list_tail;
        new_list_tail->next = node_to_replace->next;
    }
    else
        new_list_tail->next = NULL;
}

char *get_real_env(char *value)
{
	int i;

	i = 0;
	while(value[i] && value[i] != '=')
		i++;
	i++;
	return(&value[i]);
}

t_arg *env_to_arg(t_env *env_node)
{
	t_arg *head;
	t_arg *current;
	char **sp_res;
	int i;

	head = NULL;
	i = 0;
	sp_res = ft_split(get_real_env(env_node->value), ' ', GC);
	while (sp_res[i])
	{
		current = ft_lstnew_arg(NULL);
		current->content = ft_strdup(sp_res[i], GC);
		current->after_joinable = env_node->after_joinable;
		current->before_joinable = env_node->before_joinable;
		ft_lstaddback_arg(&head, current);
		i++;
	}
	return (head);
}

// t_cmd_arg *env_to_cmd_arg(t_env *env_node)
// {
// 	t_cmd_arg *head;
// 	t_cmd_arg *current;
// 	char **sp_res;
// 	t_arg *arg;
// 	int i;

// 	head = NULL;
// 	i = 0;
// 	sp_res = ft_split(get_real_env(env_node->value), ' ', GC);
// 	while(sp_res[i])
// 	{
// 		arg = ft_lstnew_arg(NULL);
// 		arg->content = ft_strdup(sp_res[i], GC);
// 		current = ft_lstnew_cmd_arg(arg);
// 		ft_lstaddback_cmd_arg(&head, current);
// 		i++;
// 	}
// 	return (head);
// }

void better_env_expander(t_arg **command, t_arg **to_replace, t_env *env)
{
	int mode;
	t_env *env_node;
	t_arg *arg;
	t_arg *hold;
	t_arg *continue_form;
	char *real_env;

	mode = (*to_replace)->to_replace;
	continue_form = (*to_replace)->next;
	env_node = get_env(env, (*to_replace)->content + 1);
	if(!env_node)
	{
		remove_arg_node(command, (*to_replace));
		(*to_replace)  = continue_form;
		return;
	}
	real_env = get_real_env(env_node->value);
	if(mode == ONLY_ENV)
	{
		(*to_replace)->content = ft_strdup(real_env, GC);
		(*to_replace)->before_joinable = true;
		(*to_replace)->after_joinable = true;
		return;
	}
	arg = env_to_arg(env_node);
	if ((*to_replace)->prev && (*to_replace)->prev->after_joinable && arg->before_joinable)
	{
		arg->content = ft_strjoin((*to_replace)->prev->content, arg->content, GC);
		remove_arg_node(command, (*to_replace)->prev);
		replace_node_with_list(command, (*to_replace), arg);
	}
	else
		replace_node_with_list(command, (*to_replace), arg);
	(*to_replace)  = continue_form;
}

void replace_cmd_arg_node(t_cmd_arg **head, t_cmd_arg *node, t_cmd_arg *new_head) 
{
	t_cmd_arg *new_tail;
    if (node == NULL || new_head == NULL || head == NULL || *head == NULL)
	{
		if (node == NULL) {
			printf("node pointer is NULL\n");
			} else if (new_head == NULL) {
				printf("new_head pointer is NULL\n");
			} else if (head == NULL) {
				printf("head pointer is NULL\n");
			} else if (*head == NULL) 
				printf("head points to NULL (list is empty)\n");
			return;
	}
	printf("Replacing %s with %s\n", node->arg->content, new_head->arg->content);
    if (node->prev != NULL) {
        node->prev->next = new_head;
        new_head->prev = node->prev;
    } else {
        *head = new_head;
        new_head->prev = NULL;
    }
	new_tail = ft_lstlast_cmd_arg(new_head);
    if (node->next != NULL) 
	{
        node->next->prev = new_tail;
        new_tail->next = node->next;
	}
    else 
        new_tail->next = NULL;
}


void remove_cmd_arg_node(t_cmd_arg **head, t_cmd_arg *node) 
{
    if (node == NULL || head == NULL || *head == NULL)
        return;
    if (node->prev != NULL)
        node->prev->next = node->next;
    else
        *head = node->next;
    if (node->next != NULL)
        node->next->prev = node->prev;
}




t_cmd_arg *env_to_cmd_arg(t_env *env_node)
{
	t_cmd_arg *head;
	t_cmd_arg *current;
	t_arg *one;
	char **sp_res;
	char *real_env;
	int i;

	head = NULL;
	i = 0;
	real_env = get_real_env(env_node->value);
	sp_res = ft_split(real_env, ' ', GC);
	while (sp_res[i])
	{
		one = ft_lstnew_arg(NULL);
		one->content = ft_strdup(sp_res[i], GC);
		current = ft_lstnew_cmd_arg(one);
		ft_lstaddback_cmd_arg(&head, current);
		i++;
	}
	return (head);
}

void arg_env_setup(t_cmd_arg **cmd_arg, t_cmd_arg *general_list, t_arg *the_env, t_env *env)
{
	int mode;
	t_env *env_node;
	t_arg *arg;
	t_cmd_arg *new_cmd_arg;
	t_cmd_arg *next;
	char *real_env;
	
	next = general_list->next;
	env_node = get_env(env, the_env->content + 1);
	if(!env_node)
	{
		remove_arg_node(&general_list->arg, the_env);
		return;
	}
	if(!env_node->after_joinable && !env_node->before_joinable)
	{
		
		// remove_arg_node(&general_list->arg, the_env);
		new_cmd_arg = env_to_cmd_arg(env_node);
		ft_lstiter_cmd_arg(new_cmd_arg);
		replace_cmd_arg_node(cmd_arg, general_list, new_cmd_arg);
	}
	(*cmd_arg) = next;
}

void arg_expander(t_arg **arg_list, t_arg **to_replace, t_env *env)
{
	t_env *env_node;
	t_arg *continue_form;
	t_arg *arg;
	int mode;

	mode = (*to_replace)->to_replace;
	continue_form = (*to_replace)->next;
	env_node = get_env(env, (*to_replace)->content + 1);
	if(!env_node)
	{
		remove_arg_node(arg_list, *to_replace);
		(*to_replace) = continue_form;
		return;
	}
	arg = env_to_arg(env_node);
	if(env_node->before_joinable && env_node->after_joinable)
	{
		replace_node_with_list(arg_list, (*to_replace), arg);
	}
	(*to_replace) = continue_form;

}