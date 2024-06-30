#include "../minishell.h"

void jump_spaces(t_lex **lex)
{
	while(*lex && (*lex)->token == WHITE_SPACE)
		(*lex) = (*lex)->next;
}

int get_arg_count(t_lex *first_arg)
{
	int arg_counter;

	arg_counter = 0;
	while(first_arg)
	{
		if(first_arg->token == WORD || first_arg->token == ENV || first_arg->token == STAR)
			arg_counter++;
		else if (first_arg->token != WORD && first_arg->token != ENV  && first_arg->token == STAR && first_arg->token != WHITE_SPACE)
			break;
		first_arg = first_arg->next;
	}
	return (arg_counter);
}

t_arg *ft_lstnew_arg(t_lex *word)
{
	t_arg *new_node;

	new_node = smart_malloc(sizeof(t_arg));
	new_node->content = NULL;
	new_node->to_replace = false;
	new_node->join_count = NONE;
	new_node->token = NONE;
	new_node->after_joinable = true;
	new_node->before_joinable = true;
	if(word)
	{
		new_node->content = ft_strdup(word->content, GC);
		new_node->to_replace = word->to_replace;
		new_node->join_count = NONE;
		new_node->token = word->token;
	}
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

t_arg *ft_lstlast_arg(t_arg *head)
{
	while(head->next)
		head = head->next;
	return (head);
}

t_arg *ft_lstfirst_arg(t_arg *tail)
{
	while(tail->prev)
		tail = tail->prev;
	return(tail);
}

int ft_lstsize_arg(t_arg *arg)
{
	int i;

	i = 0;
	while(arg)
	{
		i++;
		arg = arg->next;
	}
	return (i);
}

void ft_lstaddback_arg(t_arg **head, t_arg *new)
{
	t_arg *last_arg;
	if(!*head)
	{
		*head = new;
		return;
	}
	last_arg = ft_lstlast_arg(*head);
	last_arg->next = new;
	new->prev = last_arg;
}


void ft_lstiter_arg(t_arg *arg)
{
	while(arg)
	{
		printf("%s:%i\n", arg->content, arg->to_replace);
		arg = arg->next;
	}
}

t_arg *make_args(t_lex **first_arg)
{
    t_arg *args;
	t_arg *new_arg;
	int current_jc;
	t_lex *prev;

	if(!(*first_arg))
		return (NULL);
	current_jc = (*first_arg)->join_count;
	args = NULL;
	prev = NULL;
    while ((*first_arg)->token == WORD || (*first_arg)->token == ENV || (*first_arg)->token == STAR && (*first_arg)->join_count == current_jc)
    {
		jump_spaces(first_arg);
		new_arg = ft_lstnew_arg(*first_arg);
		ft_lstaddback_arg(&args, new_arg);
		prev = *first_arg;
        *first_arg = (*first_arg)->next;
		if(!(*first_arg))
			break;
    }
    return args;
}



// REPLACE REDIR STRING WITH REDIR ARG FOR EXPANDING
void process_redirection_token(t_lex **lex, t_middle **head, int token)
{
	t_middle *current;
	t_arg *arg;
	t_arg *arg_head;
	int current_jc;
	t_cmd_arg *after_head;
	t_cmd_arg *after_current;
	t_middle *new_cmd;

	arg_head = NULL;
    *lex = (*lex)->next;
    jump_spaces(lex);
	current_jc = (*lex)->join_count;
    current = ft_lstnew_middle(NULL, NULL, token);
	while ((*lex) && ((*lex)->token == COMMAND || (*lex)->token == ENV || (*lex)->token == STAR) && (*lex)->join_count == current_jc)
	{
		arg = ft_lstnew_arg(*lex);
		ft_lstaddback_arg(&arg_head, arg);
		// redir_string = ft_strjoin(redir_string, (*lex)->content, GC);
		(*lex) = (*lex)->next;
	}
    // current->redir_string = redir_string;
	current->redirections = arg_head;
	after_head = NULL;
	jump_spaces(lex);
	while((*lex) && (*lex)->token != AND && (*lex)->token != OR && (*lex)->token != PIPE_LINE && (*lex)->token != CLOSE_PARANTHESE && (*lex)->token != OPEN_PARANTHESE)
	{
		jump_spaces(lex);
		arg = make_args(lex);
		if(!arg)
			break;
		after_current = ft_lstnew_cmd_arg(arg);
		ft_lstaddback_cmd_arg(&after_head, after_current);
	}
    ft_lstadd_back_middle(head, current);
	if(after_head)
	{
		if(current->prev && current->prev->token == COMMAND)
		{
			if(!current->prev->cmd_arg)
				// printf("NO CMD ARG\n");
				current->prev->cmd_arg = after_head;
			else
				current->prev->cmd_arg->next = after_head;
		}
		else
		{
			new_cmd = ft_lstnew_middle(after_head->arg, after_head->next, COMMAND);
			current->prev = new_cmd;
			new_cmd->next = current;
			(*head) = new_cmd;
		}
	}
}

void process_other_token(t_lex **lex, t_middle **head)
{
	t_arg *other;
	int token;
	t_middle *current;

	other = ft_lstnew_arg((*lex));
	token = (*lex)->token;
	(*lex) = (*lex)->next;
	jump_spaces(lex);
    current = ft_lstnew_middle(other, NULL, token);
    ft_lstadd_back_middle(head, current);
}

bool is_joinable_lex(t_lex *lex)
{
	return(lex->token != AND && lex->token != OR && lex->token != PIPE_LINE && lex->token != CLOSE_PARANTHESE);
}

t_arg *make_command(t_lex **lex)
{
	t_arg *head;
	t_arg *current;
	int current_jc;

	head = NULL;
	current_jc = (*lex)->join_count;
	while((*lex) && is_joinable_lex(*lex) && (*lex)->join_count == current_jc)
	{
		current = ft_lstnew_arg((*lex));
		ft_lstaddback_arg(&head, current);
		(*lex) = (*lex)->next;
	}
	jump_spaces(lex);
	return (head);
}

bool is_there_another_arg(t_lex *lex)
{
	lex = lex->next;
	while(lex)
	{
		if(lex->token != WHITE_SPACE)
		{
			if(lex->token != COMMAND && lex->token != ENV && lex->token != STAR)
				return (false);
			return (true);
		}
		lex = lex->next;
	}
	return (false);
}

bool is_redir_lex(t_lex *lex)
{
	return(lex->token == REDIR_IN || lex->token == REDIR_OUT || lex->token == DREDIR_OUT || lex->token == HERE_DOC);
}

void process_word_token(t_lex **lex, t_middle **head, t_middle **current, bool *in_command, t_arg **command)
{
	t_arg *args;
	t_cmd_arg *cmd_head;
	t_cmd_arg *cmd_current;

	cmd_head = NULL;
    if (!(*in_command))
    {
        *command = make_command(lex);
        if (!(*lex) || (*lex)->token == AND || (*lex)->token == OR || (*lex)->token == PIPE_LINE || is_redir_lex(*lex) || (*lex)->token == CLOSE_PARANTHESE)
        {
            *current = ft_lstnew_middle(*command, NULL, COMMAND); // COMMAND IS FIRST ARG!
            ft_lstadd_back_middle(head, *current);
        }
        *in_command = true;
    }
    else
    {
		while((*lex) && (*lex)->token != AND && (*lex)->token != OR && (*lex)->token != PIPE_LINE && (*lex)->token != CLOSE_PARANTHESE && (*lex)->token != OPEN_PARANTHESE)
		{
			jump_spaces(lex);
			if(!*lex)
				break;
			args = make_args(lex);
			if(!args)
				break;
			cmd_current = ft_lstnew_cmd_arg(args);
			ft_lstaddback_cmd_arg(&cmd_head, cmd_current);
		}
		*current = ft_lstnew_middle(*command, cmd_head, COMMAND);
		ft_lstadd_back_middle(head, *current);
        *in_command = false;
    }
}

void initialize_vars(t_middle_vars *vars)
{
	vars->head = NULL;
	vars->command = NULL;
	vars->current = NULL;
	vars->in_command = false;
}

t_middle *make_middle(t_lex *lex)
{
	t_middle_vars vars;
	t_middle *last;

	initialize_vars(&vars);
    while (lex)
    {
        if (lex->token == WORD || lex->token == ENV || lex->token == STAR)
            process_word_token(&lex, &vars.head, &vars.current, &vars.in_command, &vars.command);
        else if (lex->token == REDIR_IN || lex->token == REDIR_OUT || lex->token == DREDIR_OUT || lex->token == HERE_DOC)
        {
            process_redirection_token(&lex, &vars.head, lex->token);
            vars.in_command = false;
        }
        else if (lex->token != WHITE_SPACE)
        {
            process_other_token(&lex, &vars.head);
            vars.in_command = false;
        }
		else
			jump_spaces(&lex);
    }
    return (vars.head);
}
