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

char *redirect_env_expander(char *to_expand, t_env *env)
{
	t_lex *mini_lexed;
	t_lex *original;
	t_env *env_node;
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
	me = NULL;
	while(original)
	{
		me = ft_strjoin(me, original->content, GC);
		original = original->next;
	}
	return (me);
}

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
		// if (node == NULL) {
		// 	printf("node pointer is NULL\n");
		// 	} else if (new_head == NULL) {
		// 		printf("new_head pointer is NULL\n");
		// 	} else if (head == NULL) {
		// 		printf("head pointer is NULL\n");
		// 	} else if (*head == NULL) 
		// 		printf("head points to NULL (list is empty)\n");
			return;
	}
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
		one->to_replace = env_node->star_to_replace;
		current = ft_lstnew_cmd_arg(one);
		ft_lstaddback_cmd_arg(&head, current);
		i++;
	}
	return (head);
}


char *args_to_str(t_arg *args)
{
	char *result;

	result = NULL;
	while (args)
	{
		result = ft_strjoin(result, args->content, GC);
		args = args->next;
	}
	return (result);
}

bool am_i_a_star(char *str)
{
	int i;

	i = 0;
	while(str[i])
	{
		if(str[i] == '*')
			return (true);
		i++;
	}
	return (false);
}

t_arg *final_args(t_cmd_arg *cmd_arg)
{
	t_arg *args;
	t_arg *head;
	t_arg *current;
	char *ready_arg;
	int to_replace;

	head = NULL;
	ready_arg = NULL;
	to_replace = REPLACE_ALL;
	while(cmd_arg)
	{
		args = cmd_arg->arg;
		while(args)
		{
			if(am_i_a_star(args->content) && args->to_replace < to_replace)
				to_replace = args->to_replace;
			ready_arg = ft_strjoin(ready_arg, args->content, GC);
			args = args->next;
		}
		current = ft_lstnew_arg(NULL);
		current->content = ready_arg;
		current->to_replace = to_replace;
		ft_lstaddback_arg(&head, current);
		cmd_arg = cmd_arg->next;
		ready_arg = NULL;
	}
	return (head);
}

				// if (args->to_replace == REPLACE_ALL)
				// {
				// 	star = star_matching(args->content);
				// 	if (star)
				// 	{
				// 		while (star)
				// 		{
				// 			tmp_arg = ft_lstnew_arg(NULL);
				// 			tmp_arg->content = star->value;
				// 			tmp_cmd_arg = ft_lstnew_cmd_arg(tmp_arg);
				// 			if (cmd_arg->prev)
				// 			{
				// 				cmd_arg->prev->next = tmp_cmd_arg;
				// 				tmp_cmd_arg->prev = cmd_arg->prev;
				// 			}
				// 			else
				// 				original = tmp_cmd_arg;
				// 			tmp_cmd_arg->next = cmd_arg->next;
				// 			cmd_arg->prev = tmp_cmd_arg;
				// 			star = star->next;
				// 		}
				// 	}
				// }




void insert_before_node(t_cmd_arg **head, t_cmd_arg *node, t_cmd_arg *new_node)
{
    if (!node) 
        return;

    new_node->next = node;
    new_node->prev = node->prev;
    if (node->prev)
        node->prev->next = new_node;
    else 
        *head = new_node;
    node->prev = new_node;
}


void insert_after_node(t_cmd_arg **head, t_cmd_arg *node, t_cmd_arg *new_node)
{
    if (!node) 
        return;
	// printf("Inserting %s after %s\n", new_node->arg->content, node->arg->content);
    new_node->prev = node;
    new_node->next = node->next;
    if (node->next)
        node->next->prev = new_node;
    node->next = new_node;
}

bool is_env(char *str)
{
	int i;

	if(str[0] != '$')
		return(false);
	i = 1;
	while(str[i])
	{
		if(is_special(str[i]))
			return (false);
		i++;
	}
	return (true);
}


bool is_nextable(t_arg *arg, t_env *env)
{
	t_env *env_node;

	env_node = get_env(env, arg->content  + 1);
	if(!env_node)
		return (true);
	else if(env_node->before_joinable)
		return (true);
	return (false);
}

int after_env_star(char *str)
{
	int i;
	char *res;
	
	i = 1;
	while(str[i] && str[i] != '.' && str[i] != '*')
		i++;
	if(!str[i])
		return (NONE);
	return(i);
}


void prep_cmd_arg(t_cmd_arg **cmd_arg, t_env *env)
{
	t_arg *arg;
	t_env *env_node;
	t_cmd_arg *looping_cmd;
	t_cmd_arg *next_lp_cmd;
	t_cmd_arg *set_cmd;
	t_cmd_arg *expanded_env;
	t_cmd_arg *last_expanded;
	t_arg *next;
	t_arg *prev;
	t_arg *move;
	t_arg *last_arg;
	t_arg *arg_env;
	bool go;
	int after_star;
	char *look_for;
	char *append_after;


	looping_cmd = *cmd_arg;
	while (looping_cmd)
	{
		next_lp_cmd = looping_cmd->next;
		arg = looping_cmd->arg;
		while (arg)
		{
			go = true;
			move = arg->next;
			if(arg->to_replace != NO_REPLACE && (arg->token == ENV || is_env(arg->content)))
			{
				look_for = arg->content;
				printf("I am %s\n", look_for);
				append_after = NULL;
				after_star = after_env_star(arg->content);
				if(after_star != NONE)
				{
					look_for = ft_substr(arg->content, 0, after_star, GC);
					append_after = ft_substr(arg->content, after_star, ft_strlen(arg->content), GC);
				}
				env_node = get_env(env, look_for + 1);
				if(env_node && append_after)
				{
					env_node = ft_lstnew_env(env_node->value);
					env_node->value = ft_strjoin(env_node->value, append_after, GC);
					env_node->star_to_replace = arg->to_replace;
				}
				if(!env_node)
					remove_arg_node(&looping_cmd->arg, arg);
				else if(arg->to_replace == ONLY_ENV)
					arg->content = ft_strdup(get_real_env(env_node->value), GC);
				else
				{
					prev = arg->prev;
					next = arg->next;
					expanded_env = env_to_cmd_arg(env_node);

					ft_lstiter_cmd_arg(expanded_env);
					last_expanded = ft_lstlast_cmd_arg(expanded_env);
					if(!env_node->after_joinable && !env_node->before_joinable)
					{
						if(prev)
						{
							prev->next = NULL;
							prev = ft_lstfirst_arg(prev);
							set_cmd = ft_lstnew_cmd_arg(prev);
							insert_before_node(cmd_arg, looping_cmd, set_cmd);
						}
						replace_cmd_arg_node(cmd_arg, looping_cmd, expanded_env);
						if(next)
						{
							next->prev = NULL;
							set_cmd = ft_lstnew_cmd_arg(next);
							insert_after_node(cmd_arg, last_expanded, set_cmd);
							next_lp_cmd = set_cmd;
						}
					break;
					}
					else if (env_node->before_joinable && env_node->after_joinable)
					{
						if(prev)
						{
							prev->next = NULL;
							prev = ft_lstfirst_arg(prev);
							prev->next = expanded_env->arg;
							expanded_env->arg = prev; 
						}
						replace_cmd_arg_node(cmd_arg, looping_cmd, expanded_env);
						if(next)
						{
							if(!is_nextable(next, env))
							{
								next->prev = NULL;
								set_cmd = ft_lstnew_cmd_arg(next);
								insert_after_node(cmd_arg, last_expanded, set_cmd);
								next_lp_cmd = set_cmd;
							}
							else
							{
								last_expanded->arg->next = next;
								if(is_env(next->content))
								{
									next->prev = last_expanded->arg;
									move = next;
									go = false;
									looping_cmd = last_expanded;
								}
								else
									next->prev = NULL;
							}
						}
					if(go)
						break;
					}
					else if(env_node->after_joinable)
					{
						if(prev)
						{
							prev->next = NULL;
							prev = ft_lstfirst_arg(prev);
							set_cmd = ft_lstnew_cmd_arg(prev);
							insert_before_node(cmd_arg, looping_cmd, set_cmd);
						}
						replace_cmd_arg_node(cmd_arg, looping_cmd, expanded_env);
						if(next)
						{
							if(!is_nextable(next, env))
							{
								next->prev = NULL;
								set_cmd = ft_lstnew_cmd_arg(next);
								insert_after_node(cmd_arg, last_expanded, set_cmd);
								next_lp_cmd = set_cmd;
							}
							else
							{
								last_expanded->arg->next = next;
								if(is_env(next->content))
								{
									next->prev = last_expanded->arg;
									move = next;
									go = false;
									looping_cmd = last_expanded;
								}
								else
									next->prev = NULL;
							}
						}
					if(go)
						break;
					}
					else if (env_node->before_joinable)
					{
						if(prev)
						{
							prev->next = NULL;
							prev = ft_lstfirst_arg(prev);
							prev->next = expanded_env->arg;
							expanded_env->arg = prev; 
						}
						if(next)
						{
							next->prev = NULL;
							set_cmd = ft_lstnew_cmd_arg(next);
							insert_after_node(cmd_arg, last_expanded, set_cmd);
							next_lp_cmd = set_cmd;
						}
						replace_cmd_arg_node(cmd_arg, looping_cmd, expanded_env);
					break;
					}
				}
			}
			arg = move;
		}
		looping_cmd = next_lp_cmd;
	}
}

bool is_arg_star(t_arg *arg)
{
	int i;
	char *str;

	i = 0;
	if (arg->to_replace != REPLACE_ALL)
	{
		printf("i am: %i for %s\n", arg->to_replace, arg->content);
		return (false);
	}
	str = arg->content;
	while (str[i])
	{
		if(str[i] == '*')
			return (true);
		i++;
	}
	return (false);
}

void star_expander(t_cmd_arg **cmd_arg)
{
	t_cmd_arg *looping_cmd;
	t_cmd_arg *next_lp_cmd;
	t_cmd_arg *set_cmd;
	t_cmd_arg *expanded_star;
	t_arg *arg;
	t_arg *next;
	t_arg *prev;
	t_arg *move;

	looping_cmd = *cmd_arg;
	while(looping_cmd)
	{
		next_lp_cmd = looping_cmd->next;
		arg = looping_cmd->arg;
		while(arg)
		{
			if(is_arg_star(arg))
			{
				expanded_star = cmd_arg_star_matching(arg->content);
				if(expanded_star)
				{
					next = arg->next;
					prev = arg->prev;
					if(prev)
					{
						prev->next = NULL;
						prev = ft_lstfirst_arg(prev);
						set_cmd = ft_lstnew_cmd_arg(prev);
						insert_before_node(cmd_arg, looping_cmd, set_cmd);
					}
					replace_cmd_arg_node(cmd_arg, looping_cmd, expanded_star);
					if(next)
					{
						next->prev = NULL;
						set_cmd = ft_lstnew_cmd_arg(next);
						insert_after_node(cmd_arg, expanded_star, set_cmd);
						next_lp_cmd = set_cmd;
						break;
					}
				}
			}
			arg = arg->next;
		}
		looping_cmd = next_lp_cmd;
	}
}



t_arg *expand_args(t_cmd_arg *cmd_arg, t_env *env)
{
    t_arg *args;
    t_cmd_arg *original;
	t_arg *tmp_arg;
    t_cmd_arg *tmp_cmd_arg;
	t_cmd_arg *new_cmd_arg;
    t_arg *hold_args;
    t_env *star;
	t_cmd_arg *add;
	t_cmd_arg *next_cmd_arg;

    original = cmd_arg;
	new_cmd_arg = NULL;
	prep_cmd_arg(&cmd_arg, env);
	args = final_args(cmd_arg);
	// printf("LE:");
	// ft_lstiter_cmd_arg(cmd_arg);
	// star_expander(&cmd_arg);
	expand_arg_as_star(&args);
    return (args);
}

// void expand_redirs(t_redir *redir, t_env **env, t_treenode *root)
// {
// 	t_env *star;
// 	t_arg *arg;

// 	while (redir)
// 	{
// 		arg = redir->redir_input;
// 		while(arg)
// 		{
// 			if(arg->to_replace ==)
// 			if(arg->to_replace != NO_REPLACE)
// 			{
// 				arg->content = redirect_env_expander(arg->content, *env);
// 				printf("I have: %s\n", arg->content);
// 				if(arg->to_replace == REPLACE_ALL)
// 				{
// 					star = star_matching(arg->content);
// 					if(star)
// 					{
// 						if(star->next)
// 						{
// 							ft_putstr_fd(2, redir->redir_string);
// 							ft_putstr_fd(2, ": ambiguous redirect\n");
// 							change_status(env, 1);
// 							init_tree(root);
// 							return;
// 						}
// 						else
// 							arg->content = ft_strdup(star->value, GC);
// 					}
// 				}
// 			}
// 			arg = arg->next;
// 		}
// 		arg = redir->redir_input;
// 		while(arg)
// 		{
// 			redir->redir_string = ft_strjoin(redir->redir_string, arg->content, GC);
// 			arg = arg->next;
// 		}
// 		redir = redir->next;
// 	}
// }




bool is_builtin(char *command)
{
	return (!ft_strcmp(command, "env") || !ft_strcmp(command, "echo") || !ft_strcmp(command, "pwd") || !ft_strcmp(command, "export") || !ft_strcmp(command, "unset") || !ft_strcmp(command, "cd") || !ft_strcmp(command, "exit"));
}


			// 	command->content = env_expander(command->content, (*env));
			// no_star = no_stars(command->content);
			// if(command->to_replace == REPLACE_ALL)
			// {
			// 	star = star_matching(command->content);
			// 	if(star)
			// 	{
			// 		command->content = star->value;
			// 		star = star->next;
			// 	}
			// 	while(star)
			// 	{
			// 		tmp_arg = ft_lstnew_arg(NULL);
			// 		tmp_arg->content = ft_strdup((*env)->value, GC);
			// 		ft_lstaddback_arg(&args, tmp_arg);
			// 		star = star->next;
			// 	}
			// }



void expand_arg_as_star(t_arg **head)
{
	t_arg *arg;
	t_arg *tmp_arg;
	t_arg *next;

	arg = *head;
	while(arg && arg->to_replace == REPLACE_ALL)
	{
		next = arg->next;
		tmp_arg = arg_star_matching(arg->content);
		if(tmp_arg)
		{
			replace_node_with_list(head, arg, tmp_arg);
			next = tmp_arg->next;
		}
		arg = next;
	}

}


void expand_node(t_treenode *root, t_env **env)
{
	t_arg *command;
	t_env *star;
	t_arg *args;
	char *no_star;
	t_arg *tmp_arg;
	t_arg *next;
	int changers;

	command = root->command;
	args = NULL;
	tmp_arg = NULL;
	no_star = NULL;
	if(command)
	{
		while(command)
		{
			if(command->to_replace != NO_REPLACE && (command->token == ENV || is_env(command->content)))
				better_env_expander(&root->command, &command, *env);
			else
				command = command->next;
		}
		if(!root->command)
		{
			root->command = ft_lstnew_arg(NULL);
			return;
		}
		expand_arg_as_star(&root->command);
		root->content = root->command->content;
		no_star = no_stars(root->content);
		if(no_star && is_path(no_star) && is_a_directory(no_star))
		{
			ft_putstr_fd(2, no_star);
			ft_putstr_fd(2,": Is a directory\n");
			root->is_a_directory = true;
			root->content = ft_strdup(no_star, GC);
			change_status(env, DIRECORY_STATUS);
			return;
		}
		tmp_arg = root->command->next;
		root->command->next = NULL;
	}
	root->args = tmp_arg;
	if(!tmp_arg)
		root->args = expand_args(root->cmd_arg, *env);
	else
	{
		tmp_arg->next = expand_args(root->cmd_arg, *env);
		root->args = tmp_arg;
	}
	expand_arg_as_star(&root->args);


	// expand_redirs(root->before_redir, env, root);
	// expand_redirs(root->after_redir, env, root);
}