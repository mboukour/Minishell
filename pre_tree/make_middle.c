#include "../minishell.h"

int get_arg_count(t_lex *first_arg)
{
	int arg_counter;

	arg_counter = 0;
	while(first_arg)
	{
		if(first_arg->token == WORD)
			arg_counter++;
		else if (first_arg->token != WORD && first_arg->token != WHITE_SPACE)
			break;
		first_arg = first_arg->next;
	}
	return (arg_counter);
}

char **make_args(t_lex **first_arg)
{
	int arg_count;
	char **args;
	int i;
	t_lex *prev;

	arg_count = get_arg_count(*first_arg);
	args = smart_malloc((arg_count + 1) * sizeof(char *));
	i = 0;
	while(i < arg_count)
	{
		if((*first_arg)->token == WORD)
		{
			args[i] = ft_strdup((*first_arg)->content);
			i++;
		}
		prev = *first_arg;
		*first_arg = (*first_arg)->next;
		if(!(*first_arg) || ((*first_arg)->token != WHITE_SPACE && (*first_arg)->token != WORD))
		{
			*first_arg = prev;
			break;
		}
	}
	args[i] = NULL;
	return (args);
}

bool do_i_have_args(t_lex *command)
{
	int word_count;

	word_count = 0;
	char *cmd = command->content;
	while(command && (command->token == WORD || command->token == WHITE_SPACE))
	{
		if(command->token == WORD)
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

    current = ft_lstnew_middle(ft_strdup((*lex)->content), NULL, token);
    *lex = (*lex)->next;
    jump_spaces(lex);
    current->redir_string = ft_strdup((*lex)->content);
    ft_lstadd_back_middle(head, current);
}

void process_other_token(t_lex **lex, t_middle **head)
{
	t_middle *current;

    current = ft_lstnew_middle(ft_strdup((*lex)->content), NULL, (*lex)->token);
    ft_lstadd_back_middle(head, current);
}
void process_word_token(t_lex **lex, t_middle **head, t_middle **current, bool *in_command, char **command, char ***args)
{
    if (!(*in_command))
    {
        *command = ft_strdup((*lex)->content);
        *in_command = true;
        if (!do_i_have_args(*lex))
        {
            *current = ft_lstnew_middle(*command, NULL, COMMAND);
            ft_lstadd_back_middle(head, *current);
        }
    }
    else
    {
        *args = make_args(lex);
        *current = ft_lstnew_middle(*command, *args, COMMAND);
        ft_lstadd_back_middle(head, *current);
        *in_command = false;
    }
}

void initialize_vars(t_middle_vars *vars)
{
	vars->head = NULL;
	vars->args = NULL;
	vars->command = NULL;
	vars->current = NULL;
	vars->in_command = false;
}

t_middle *make_middle(t_lex *lex)
{
	t_middle_vars vars;

	initialize_vars(&vars);
    while (lex)
    {
        if (lex->token == WORD)
            process_word_token(&lex, &vars.head, &vars.current, &vars.in_command, &vars.command, &vars.args);
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
    return vars.head;
}
