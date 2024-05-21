#include "../minishell.h"


// bool check_parantheses(t_lex *tokens)
// {
// 	while(tokens)
// 	{
	
// 	}
// }
t_lex *lex_input_checker(t_lex *tokens)
{
	t_lex *prev;
	t_lex *next;

    while (tokens)
    {
        if (tokens->token == AND || tokens->token == OR || tokens->token == PIPE_LINE)
        {
            prev = tokens->prev;
            while (prev && prev->token == WHITE_SPACE)
                prev = prev->prev;
            if (!prev || (prev->token != WORD && prev->token != QUOTE && prev->token != DOUBLE_QUOTE && prev->token != CLOSE_PARANTHESE))
                return tokens;
            next = tokens->next;
            while (next && next->token == WHITE_SPACE)
                next = next->next;
            if (!next || (next->token != WORD && next->token != QUOTE && next->token != DOUBLE_QUOTE && next->token != OPEN_PARANTHESE))
                return tokens;
        }
        tokens = tokens->next;
    }
    return NULL;
}
