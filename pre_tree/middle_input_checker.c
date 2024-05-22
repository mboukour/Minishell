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
		middled = middled->next;
	}
	return (NULL);
}