#include "env_management.h"



t_cmd_arg *env_to_cmd_arg(t_env *env_node)
{
	t_cmd_arg *head;
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
			handle_splitted_env(&head, sp_res[i], env_node);
			i++;
		}
	}
	else
	{
		head = ft_lstnew_cmd_arg(ft_lstnew_arg(NULL));
		head->arg->content = ft_strdup(env_node->value, GC);
	}
	return (head);
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
