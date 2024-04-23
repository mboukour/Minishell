#include "../minishell.h"



token get_token_type(char c) 
{
    if (c == ' ') 
        return WHITE_SPACE;
     else if (c == '\n') 
        return NEW_LINE;
 	else if (c == '\'') 
        return QUOTE;
     else if (c == '\"') 
        return DOUBLE_QUOTE;
     else if (c == '\\') 
        return ESCAPE;
     else if (c == '$') 
        return ENV;
     else if (c == '|') 
        return PIPE_LINE;
     else if (c == '<') 
        return REDIR_IN;
     else if (c == '>') 
        return REDIR_OUT;
     else 
        return WORD;
}
