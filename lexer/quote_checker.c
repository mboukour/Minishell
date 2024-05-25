#include "../minishell.h"



bool quote_checker(t_lex *token)
{
	bool in_quotes;
	bool in_dquotes;

	in_quotes = false;
	in_dquotes = false;

	while(token)
	{
		if (token->token == DOUBLE_QUOTE && !(in_quotes))
			in_dquotes = !(in_dquotes);
		else if (token->token == QUOTE && !(in_dquotes))
			in_quotes = !(in_quotes);
		token = token->next;
	}
	return (in_dquotes || in_quotes);
}