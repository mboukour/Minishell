#include "../minishell.h"


void handle_star(t_lex *lexed) // HANDLE STAR MATCHING
{
	DIR *current;
    struct dirent *read;
    char *ret;
	int last_type;

	ret = NULL;
	current = opendir(".");
	if(!current)
		return ; // ERROR MANAGEMENT
	read = readdir(current);
	while (read != NULL)
	{
		if (read->d_type == DT_REG || read->d_type == DT_DIR && read->d_name[0] != '.')
		{
			ret = ft_strjoin(ret, read->d_name, GC);
			last_type = read->d_type;
		}
		read = readdir(current);
		if(read && (last_type == DT_REG || last_type == DT_DIR))
			ret = ft_strjoin(ret, " ", GC);
	}
	closedir(current);
	lexed->content = ret;
	lexed->token = WORD;
}


void handle_quotes(t_lex *lex)
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
	if(original->token == DOUBLE_QUOTE)
		original->to_replace = true;
	original->token = WORD;
	original->next = looping->next;
}

void expand(t_lex *lexed, int mode)
{
	while(lexed)
	{
		if (lexed->token == STAR && mode == STAR)
			handle_star(lexed);
		else if((lexed->token == QUOTE || lexed->token == DOUBLE_QUOTE) && mode == QUOTE)
			handle_quotes(lexed);
		lexed = lexed->next;
	}
}
