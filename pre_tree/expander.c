#include "../minishell.h"


void expand_quotes(t_lex *lex)
{
	char *str;
	t_lex *looping;
	t_lex *original;

	original = lex;
	str = NULL;
	lex = lex->next;
	looping = lex;
	while(looping && looping->token != original->token)
		looping = looping->next;
	while(lex && lex != looping)
	{
		str = ft_strjoin(str, lex->content, GC);
		lex = lex->next;
	}
	if(!str)
		str = ft_strdup("", GC);
	original->content = str;
	original->to_replace = REPLACE_ALL;
	if(original->token == DOUBLE_QUOTE)
		original->to_replace = ONLY_ENV;
	else if(original->token == QUOTE)
		original->to_replace = NO_REPLACE;
	original->token = WORD;
	original->next = looping->next;
}

/// QUOTES NOT INTERPTED CORRECTLY  'cat''>>' ===> cat>>
void quotes_handler(t_lex *lexed)
{
	while(lexed)
	{
		if(lexed->token == QUOTE || lexed->token == DOUBLE_QUOTE)
			expand_quotes(lexed);
		lexed = lexed->next;
	}
}
