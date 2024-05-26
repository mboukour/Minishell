#include "../minishell.h"

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
	if(word)
	{
		new_node->content = ft_strdup(word->content, GC);
		new_node->to_replace = word->to_replace;
	}
	new_node->next = NULL;
	return (new_node);
}

t_arg *ft_lstlast_arg(t_arg *head)
{
	while(head->next)
		head = head->next;
	return (head);
}

void ft_lstaddback_arg(t_arg **head, t_arg *new)
{
	if(!*head)
	{
		*head = new;
		return;
	}
	ft_lstlast_arg(*head)->next = new;
}

t_arg *make_args(t_lex **first_arg)
{
    t_arg *args;
    t_lex *prev;
	t_arg *new_arg;

	args = NULL;
	prev = NULL;
    while (*first_arg)
    {
        if ((*first_arg)->token == WORD || (*first_arg)->token == ENV || (*first_arg)->token == STAR)
        {
            new_arg = ft_lstnew_arg(*first_arg);
            ft_lstaddback_arg(&args, new_arg);
        }
        prev = *first_arg;
        *first_arg = (*first_arg)->next;
        if (!(*first_arg) || ((*first_arg)->token != WHITE_SPACE && (*first_arg)->token != WORD && (*first_arg)->token != ENV && (*first_arg)->token != STAR) && (*first_arg)->token != STAR)
        {
            *first_arg = prev;
            break;
        }
    }
    
    return args;
}

bool do_i_have_args(t_lex *command)
{
	int word_count;
	char *cmd;

	word_count = 0;
	cmd = command->content;
	while(command && (command->token == WORD || command->token == ENV  || command->token == STAR || command->token == WHITE_SPACE))
	{
		if(command->token != WHITE_SPACE)
			word_count++;
		command = command->next;
	}
	if(word_count > 1)
		return (true);
	return (false);
}


void jump_spaces(t_lex **lex)
{
	while(*lex && (*lex)->token == WHITE_SPACE)
		(*lex) = (*lex)->next;
}

void process_redirection_token(t_lex **lex, t_middle **head, int token)
{
	t_middle *current;

    current = ft_lstnew_middle(ft_strdup((*lex)->content, GC), NULL, token);
    *lex = (*lex)->next;
    jump_spaces(lex);
    current->redir_string = ft_strdup((*lex)->content, GC);
	current->to_replace = (*lex)->to_replace;
    ft_lstadd_back_middle(head, current);
}

void process_other_token(t_lex **lex, t_middle **head)
{
	int token;
	t_middle *current;

    current = ft_lstnew_middle(ft_strdup((*lex)->content, GC), NULL, (*lex)->token);
    ft_lstadd_back_middle(head, current);
}
void process_word_token(t_lex **lex, t_middle **head, t_middle **current, bool *in_command, char **command, bool *to_replace)
{
	t_arg *args;

    if (!(*in_command))
    {
        *command = ft_strdup((*lex)->content, GC);
        *in_command = true;
		*to_replace = (*lex)->to_replace;
        if (!do_i_have_args(*lex))
        {
            *current = ft_lstnew_middle(*command, NULL, COMMAND);
			(*current)->to_replace = *to_replace;
            ft_lstadd_back_middle(head, *current);
        }
    }
    else
    {
        args = make_args(lex);
        *current = ft_lstnew_middle(*command, args, COMMAND);
		(*current)->to_replace = *to_replace;
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
	vars->to_replace = false;
}



t_middle *make_middle(t_lex *lex)
{
	t_middle_vars vars;
	t_middle *last;

	initialize_vars(&vars);
    while (lex)
    {
        if (lex->token == WORD || lex->token == ENV || lex->token == STAR)
            process_word_token(&lex, &vars.head, &vars.current, &vars.in_command, &vars.command, &vars.to_replace);
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
        lex = lex->next;
    }
    return (vars.head);
}
