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

	i = 0;
	arg_count = get_arg_count(*first_arg);
	args = malloc((arg_count + 1) * sizeof(char *));
	while(first_arg && i < arg_count)
	{
		if((*first_arg)->token == WORD)
		{
			args[i] = ft_strdup((*first_arg)->content);
			i++;
		}
		else if((*first_arg)->token != WORD && (*first_arg)->token != WHITE_SPACE)
		{
			if(i != arg_count - 1) // DEBUGGING IF STATEMENT
			{
				printf("ERROR: DIDNT GO OVER EVERYTHING!\n");
				exit(1);
			}
			break;
		}
		*first_arg = (*first_arg)->next;
	}
	args[i] = NULL;
	return (args);
}

t_middle *make_middle(t_lex *lex)
{
	t_middle *head;
	t_middle *current;
	bool in_command;
	char *command;
	char **args;

	command = NULL;
	head = NULL;
	args = NULL;
	current = NULL;
	in_command = false;
	while(lex)
	{
		if(lex->token == WORD)
		{
			{
			if(!in_command)
			{
				command = ft_strdup(lex->content);
				in_command = true;
			}
			else
			{
				args = make_args(&lex);
				current = ft_lstnew_middle(command, args, COMMAND);
				ft_lstadd_back_middle(&head, current);
				in_command = false;
			}
			}
		}
		else if(lex->token != WHITE_SPACE)
		{
			current = ft_lstnew_middle(command, args, lex->token);
			ft_lstadd_back_middle(&head, current);
		}
		lex = lex->next;
	}
	return (head);
}