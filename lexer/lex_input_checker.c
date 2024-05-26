#include "../minishell.h"


bool is_valid_prev_token(t_lex *token)
{
    while (token && token->token == WHITE_SPACE)
        token = token->prev;
    if (!token || (token->token != WORD && token->token != ENV && token->token != STAR &&
                   token->token != QUOTE && token->token != DOUBLE_QUOTE && token->token != CLOSE_PARANTHESE))
        return false;
    return true;
}

bool is_valid_next_token(t_lex *token)
{
    while (token && token->token == WHITE_SPACE)
        token = token->next;
    if (!token || (token->token != WORD && token->token != ENV && token->token != STAR &&
                   token->token != QUOTE && token->token != DOUBLE_QUOTE && token->token != OPEN_PARANTHESE))
        return false;
    return true;
}

t_lex *check_logical_operators(t_lex *tokens)
{
    if (!is_valid_prev_token(tokens->prev) || !is_valid_next_token(tokens->next))
        return tokens;
    return NULL;
}

t_lex *check_redirections(t_lex *tokens)
{
    if (!is_valid_next_token(tokens->next))
        return tokens;
    return NULL;
}

void false_syntax(t_bool_syntax *syntax)
{
    syntax->in_dquote = false;
    syntax->in_quote = false;
    syntax->in_para = false;
}
t_lex* skip_sp(t_lex *lex, int mode)
{
    if(mode == 0)
    {
        lex = lex->prev;
        while(lex && lex->token == WHITE_SPACE)
            lex = lex->prev;
        return (lex);
    }
    else
    {
        lex = lex->next;
        while(lex && lex->token == WHITE_SPACE)
            lex = lex->next;
        return (lex);
    }
}

t_lex *check_parentheses(t_lex *tokens)
{
    t_lex *next;
    t_lex *prev;

    next = skip_sp(tokens, 1);
    prev = skip_sp(tokens, 0);
    if (tokens->token == OPEN_PARANTHESE)
    {
        if (prev && (prev->token != AND && prev->token != OR && prev->token != PIPE_LINE))
            return tokens;
        if (!next)
            return tokens;
        else if (next->token == CLOSE_PARANTHESE)
            return next;
    }
    else if (tokens->token == CLOSE_PARANTHESE)
    {
        if (next && (next->token != AND && next->token != OR && next->token != PIPE_LINE))
            return tokens;
    }
    return NULL;
}

t_lex *lex_input_checker(t_lex *tokens)
{
    t_bool_syntax syntax;
    t_lex *error_token;

    false_syntax(&syntax);
    while (tokens)
    {
        if ((tokens->token == AND || tokens->token == OR || tokens->token == PIPE_LINE) && (!syntax.in_quote && !syntax.in_dquote))
        {
            error_token = check_logical_operators(tokens);
            if (error_token)
                return error_token;
        }
        else if ((tokens->token == REDIR_IN || tokens->token == REDIR_OUT || tokens->token == DREDIR_OUT || tokens->token == HERE_DOC) && (!syntax.in_quote && !syntax.in_dquote))
        {
            error_token = check_redirections(tokens);
            if (error_token)
                return error_token;
        }
        else if (tokens->token == OPEN_PARANTHESE || tokens->token == CLOSE_PARANTHESE)
        {
            if (!syntax.in_quote && !syntax.in_dquote)
            {
                error_token = check_parentheses(tokens);
                if (error_token)
                    return error_token;
            }
        }
        tokens = tokens->next;
    }
    return (NULL);
}

