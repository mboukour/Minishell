#include "../minishell.h"

t_lex *lex_input_checker(t_lex *tokens, int *error_checker)
{
    t_lex *prev;
    t_lex *next;
    bool in_dquotes;
    bool in_quotes;
    bool in_para;

    in_para = false;
    in_quotes = false;
    in_dquotes = false;
    while (tokens)
    {
        if ((tokens->token == AND || tokens->token == OR || tokens->token == PIPE_LINE) && (!in_quotes && !in_dquotes))
        {
            prev = tokens->prev;
            while (prev && prev->token == WHITE_SPACE)
                prev = prev->prev;
            if (!prev || (prev->token != WORD && prev->token != ENV && prev->token != STAR && prev->token != QUOTE &&
                prev->token != DOUBLE_QUOTE && prev->token != CLOSE_PARANTHESE))
                return tokens;
            next = tokens->next;
            while (next && next->token == WHITE_SPACE)
                next = next->next;
            if (!next || (next->token != WORD && next->token != ENV && next->token != STAR && next->token != QUOTE &&
                next->token != DOUBLE_QUOTE && next->token != OPEN_PARANTHESE))
                return tokens;
        }
        else if ((tokens->token == REDIR_IN || tokens->token == REDIR_OUT || tokens->token == DREDIR_OUT || tokens->token == HERE_DOC) && (!in_quotes && !in_dquotes))
        {
            next = tokens->next;
            while (next && next->token == WHITE_SPACE)
                next = next->next;
            if (!next || (next->token != WORD && next->token != ENV && next->token != STAR))
                return tokens;
        }
        else if (tokens->token == DOUBLE_QUOTE && !in_quotes)
            in_dquotes = !in_dquotes;
        else if (tokens->token == QUOTE && !in_dquotes)
            in_quotes = !in_quotes;
        else if((tokens->token == OPEN_PARANTHESE || tokens->token == CLOSE_PARANTHESE) && (!in_quotes && !in_dquotes))
            in_para = !in_para;
        tokens = tokens->next;
    }
    if (in_quotes || in_dquotes)
        *error_checker = QUOTE;
    else if(in_para)
        *error_checker = OPEN_PARANTHESE;
    else
        *error_checker = false;
    return (NULL);
}
