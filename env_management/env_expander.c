#include "../minishell.h"


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
	sp_res = ft_split_ws(get_real_env(env_node->value), GC);
	while (sp_res[i])
	{
		current = ft_lstnew_arg(NULL);
		current->content = ft_strdup(sp_res[i], GC);
		current->after_joinable = env_node->after_joinable;
		current->before_joinable = env_node->before_joinable;
		current->to_replace = env_node->star_to_replace;
		ft_lstaddback_arg(&head, current);
		i++;
	}
	return (head);
}




int after_env_star(char *str)
{
	int i;
	char *make_sure;

	
	i = 1;
	while(str[i] && str[i] != '.' && str[i] != '*' && str[i] != '/')
		i++;
	if(!str[i])
		return (NONE);
	make_sure = ft_substr(str, 0, i, GC);
	if(!is_env(make_sure))
		return(NONE);
	return(i);
}



void replace_cmd_arg_node(t_cmd_arg **head, t_cmd_arg *node, t_cmd_arg *new_head) 
{
	t_cmd_arg *new_tail;
    if (node == NULL || new_head == NULL || head == NULL || *head == NULL)
	{
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
	sp_res = ft_split_ws(real_env, GC);
	if(sp_res)
	{
		while (sp_res[i])
		{
			one = ft_lstnew_arg(NULL);
			one->content = ft_strdup(sp_res[i], GC);
			one->to_replace = env_node->star_to_replace;
			current = ft_lstnew_cmd_arg(one);
			ft_lstaddback_cmd_arg(&head, current);
			i++;
		}
	}
	else
	{
		one = ft_lstnew_arg(NULL);
		one->content = ft_strdup(env_node->value, GC);
		head = ft_lstnew_cmd_arg(one);
	}
	return (head);
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
		if(ready_arg)
		{
			current = ft_lstnew_arg(NULL);
			current->content = ready_arg;
			current->to_replace = to_replace;
			ft_lstaddback_arg(&head, current);
		}
		cmd_arg = cmd_arg->next;
		ready_arg = NULL;
	}
	return (head);
}





void insert_before_cmd_arg(t_cmd_arg **head, t_cmd_arg *node, t_cmd_arg *new_node)
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


void insert_after_cmd_arg(t_cmd_arg *node, t_cmd_arg *new_node)
{
    if (!node) 
        return;
    new_node->prev = node;
    new_node->next = node->next;
    if (node->next)
        node->next->prev = new_node;
    node->next = new_node;
}

bool is_env(char *str)
{
	int i;

	if(str[0] != '$' || !str[1])
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


char *lex_to_str(t_lex *lex)
{
	char *result;

	result = NULL;
	while(lex)
	{
		result = ft_strjoin(result, lex->content, GC);
		lex = lex->next;
	}
	return (result);
}

void expand_only_env(t_arg **arg_head, t_env *env)
{
	char *look_for;
	char *append_after;
	t_arg *arg;
	t_lex *broken;
	t_lex *hold;
	t_lex *next;
	t_env *env_node;
	int after_star;
	bool changed;

	changed = false;
	append_after = NULL;
	arg = *arg_head;
	if(arg->to_replace == ONLY_ENV)
	{

		broken = tokenizer(arg->content);
		hold = broken;
		while(broken)
		{
			next = broken->next;
			if(broken->token == ENV)
			{
				look_for = broken->content;
				after_star = after_env_star(broken->content);
				if(after_star != NONE)
				{
					look_for = ft_substr(broken->content, 0, after_star, GC);
					append_after = ft_substr(broken->content, after_star, ft_strlen(broken->content), GC);
				}
				changed = true;
				env_node = get_env(env, look_for + 1);
				if(env_node && env_node->envyable)
					broken->content = ft_strjoin(get_real_env(env_node->value), append_after, GC);
				else
					remove_lex_node(&hold, broken);
			}
			broken = next;
		}
		if(!changed)
			return;
		arg->content = lex_to_str(hold);
		if(!arg->content)
			remove_arg_node(arg_head, arg);
	}
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
			after_star = after_env_star(arg->content);
			if(arg->to_replace == REPLACE_ALL && (arg->token == ENV || is_env(arg->content) || after_star != NONE))
			{
				if(arg->content[1] && is_c_num(arg->content[1]))
				{
					arg->content += 2;
					arg = move;
					continue;
				}
				look_for = arg->content;
				append_after = NULL;
				if(after_star != NONE)
				{
					look_for = ft_substr(arg->content, 0, after_star, GC);
					append_after = ft_substr(arg->content, after_star, ft_strlen(arg->content), GC);
				}
				env_node = get_env(env, look_for + 1);
				if(env_node && append_after)
				{
					env_node = ft_lstnew_env(env_node->value, GC);
					env_node->value = ft_strjoin(env_node->value, append_after, GC);
					env_node->star_to_replace = arg->to_replace;
				}
				if(!env_node && append_after)
				{
					arg->content = append_after;
					break;
				}
				else if(!env_node || !env_node->envyable)
				{
					remove_arg_node(&looping_cmd->arg, arg);
					arg = move;
					continue;
				}
				else
				{
					prev = arg->prev;
					next = arg->next;
					expanded_env = env_to_cmd_arg(env_node);
					last_expanded = ft_lstlast_cmd_arg(expanded_env);
					if(!env_node->after_joinable && !env_node->before_joinable)
					{
						if(prev)
						{
							prev->next = NULL;
							prev = ft_lstfirst_arg(prev);
							set_cmd = ft_lstnew_cmd_arg(prev);
							insert_before_cmd_arg(cmd_arg, looping_cmd, set_cmd);
						}
						replace_cmd_arg_node(cmd_arg, looping_cmd, expanded_env);
						if(next)
						{
							next->prev = NULL;
							set_cmd = ft_lstnew_cmd_arg(next);
							insert_after_cmd_arg(last_expanded, set_cmd);
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
							expanded_env->arg->prev = prev;
							expanded_env->arg = prev;
						}
						replace_cmd_arg_node(cmd_arg, looping_cmd, expanded_env);
						if(next)
						{
							if(!is_nextable(next, env))
							{
								next->prev = NULL;
								set_cmd = ft_lstnew_cmd_arg(next);
								insert_after_cmd_arg(last_expanded, set_cmd);
								next_lp_cmd = set_cmd;
							}
							else
							{
								ft_lstlast_arg(last_expanded->arg)->next = next;
								if(is_env(next->content))
								{
									next->prev = ft_lstlast_arg(last_expanded->arg);
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
							insert_before_cmd_arg(cmd_arg, looping_cmd, set_cmd);
						}
						replace_cmd_arg_node(cmd_arg, looping_cmd, expanded_env);
						if(next)
						{
							if(!is_nextable(next, env))
							{
								next->prev = NULL;
								set_cmd = ft_lstnew_cmd_arg(next);
								insert_after_cmd_arg(last_expanded, set_cmd);
								next_lp_cmd = set_cmd;
							}
							else
							{
								ft_lstlast_arg(last_expanded->arg)->next = next;
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
							insert_after_cmd_arg(last_expanded, set_cmd);
							next_lp_cmd = set_cmd;
						}
						replace_cmd_arg_node(cmd_arg, looping_cmd, expanded_env);
					break;
					}
				}
			}
			else if(arg->to_replace == ONLY_ENV)
				expand_only_env(&looping_cmd->arg, env);
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
		return (false);
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
						insert_before_cmd_arg(cmd_arg, looping_cmd, set_cmd);
					}
					replace_cmd_arg_node(cmd_arg, looping_cmd, expanded_star);
					if(next)
					{
						next->prev = NULL;
						set_cmd = ft_lstnew_cmd_arg(next);
						insert_after_cmd_arg(expanded_star, set_cmd);
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

	prep_cmd_arg(&cmd_arg, env);
	args = final_args(cmd_arg);
    return (args);
}



void expand_arg_as_star(t_arg **head)
{
	t_arg *arg;
	t_arg *tmp_arg;
	t_arg *next;

	arg = *head;
	while(arg && (arg->to_replace == REPLACE_ALL))
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

int get_to_replace(t_arg *arg)
{
	int to_replace;

	to_replace = REPLACE_ALL;
	while(arg)
	{
		if(is_arg_star(arg) && arg->to_replace < to_replace)
			to_replace = arg->to_replace;
		arg = arg->next;
	}
	return (to_replace);
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

int get_least_replace(t_arg *args)
{
	int to_replace;

	to_replace = REPLACE_ALL;
	while (args)
	{
		if(args->to_replace < to_replace)
			to_replace = args->to_replace;
		args = args->next;
	}
	return (to_replace);
}

void expand_redirs(t_redir *redir, t_env **env, t_treenode *root)
{
	t_cmd_arg *for_redir;
	t_arg *arg;

	while (redir)
	{
		if(redir->token != HERE_DOC)
		{
			for_redir = ft_lstnew_cmd_arg(redir->redir_input);
			arg = expand_args(for_redir, *env);
			expand_arg_as_star(&arg);
			if(!arg || arg->next)
			{
				ft_putstr_fd(2, args_to_str(redir->redir_input));
				ft_putstr_fd(2, ": ambiguous redirect\n");
				export_core(env, "?=1");
				init_tree(root);
				return;
			}
			redir->redir_string = args_to_str(arg);
			redir->here_doc_replacer = get_least_replace(redir->redir_input);
		}
		else
		{
			redir->redir_string = args_to_str(redir->redir_input);
			redir->here_doc_replacer = get_least_replace(redir->redir_input);
		}
		redir = redir->next;
	}
	
}


void expand_node(t_treenode *root, t_env **env)
{
	char *no_star;
	t_arg *tmp_arg;
	t_cmd_arg *for_command;

	for_command = ft_lstnew_cmd_arg(root->command);
	root->command = expand_args(for_command, *env);
	if(!root->command)
	{
		root->command = ft_lstnew_arg(NULL);
		export_core(env, "?=0");
	}
	else
	{
		expand_arg_as_star(&root->command);
		root->content = root->command->content;
		no_star = no_stars(root->content);
		if(no_star && is_path(no_star) && is_a_directory(no_star))
		{
			root->is_a_directory = true;
			root->content = ft_strdup(no_star, GC);
		}
		tmp_arg = root->command->next;
		root->command->next = NULL;
		root->args = tmp_arg;
		if(!tmp_arg)
			root->args = expand_args(root->cmd_arg, *env);
		else
		{
			tmp_arg->next = expand_args(root->cmd_arg, *env);
			root->args = tmp_arg;
		}
		expand_arg_as_star(&root->args);
	}
	expand_redirs(root->before_redir, env, root);
	expand_redirs(root->after_redir, env, root);
}