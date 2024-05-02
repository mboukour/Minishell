#include "../minishell.h"


// bool check_parantheses(t_lex *tokens)
// {
// 	while(tokens)
// 	{
	
// 	}
// }

t_lex *check_valid_input(t_lex *tokens)
{
	while(tokens)
	{
		if(tokens->token == AND || tokens->token == OR || tokens->token == PIPE_LINE)
		{
			if (!tokens->prev || !tokens->prev->prev || 
				tokens->prev->token != WHITE_SPACE || 
				(tokens->prev->prev->token != WORD && 
				tokens->prev->prev->token != QUOTE && 
				tokens->prev->prev->token != DOUBLE_QUOTE))
				return (tokens);
			else if (!tokens->next || !tokens->next->next || 
					tokens->next->token != WHITE_SPACE || 
					(tokens->next->next->token != WORD && 
					tokens->next->next->token != QUOTE && 
					tokens->next->next->token != DOUBLE_QUOTE))
				return (tokens);

		}
		tokens = tokens->next;
	}
	return (NULL);
}