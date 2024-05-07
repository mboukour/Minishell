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

	i = 0;
	arg_count = get_arg_count(*first_arg);
	args = malloc((arg_count + 1) * sizeof(char *));
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
					if(!do_i_have_args(lex))
					{
						current = ft_lstnew_middle(command, NULL, COMMAND);
						ft_lstadd_back_middle(&head, current);
					}
				}
				else
				{
					args = make_args(&lex);
					current = ft_lstnew_middle(command, args, COMMAND);
					ft_lstadd_back_middle(&head, current);
					in_command = false;
					if(!lex)
					{
						printf("Last before my null: %s\n", command);
						break;
					}
				}
			}
		}
		else if(lex->token != WHITE_SPACE)
		{
			// printf("HI: %s\n", lex->content);
			current = ft_lstnew_middle(ft_strdup(lex->content), NULL, lex->token);
			ft_lstadd_back_middle(&head, current);
			in_command = false;
		}
		lex = lex->next;
	}
	return (head);
}