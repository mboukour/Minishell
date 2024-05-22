#include "../minishell.h"


t_middle *middle_input_checker(t_middle *middled)
{
	t_middle *next;

	while(middled)
	{
		next = middled->next;
		if(middled->token == OPEN_PARANTHESE)
		{
			if(!next)
				return (middled);
			else if(next->token == CLOSE_PARANTHESE)
				return (next);
		}
		else if(middled->token == REDIR_IN || middled->token == REDIR_OUT || middled->token == DREDIR_OUT || middled->token == DREDIR_OUT)
		{
			if(!next)	
				return (middled);
			else if (next->token != COMMAND)
				return (next);
		}
		middled = middled->next;
	}
	return (NULL);
}