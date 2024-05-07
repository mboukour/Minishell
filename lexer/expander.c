#include "../minishell.h"


char *free_and_join(char *s1, char *s2)
{
	char *ret;
	char *tmp;

	tmp = s1;
	ret = ft_strjoin(s1, s2);
	free(tmp);
	return (ret);
}

char *handle_star(t_lex *lex) // HANDLE STAR MATCHING
{
	DIR *current;
    struct dirent *read;
    char *ret;
	int last_type;

	ret = NULL;
	current = opendir(".");
	if(!current)
		return (NULL);
	read = readdir(current);
	while (read != NULL)
	{
		if (read->d_type == DT_REG || read->d_type == DT_DIR && read->d_name[0] != '.')
		{
			ret = free_and_join(ret, read->d_name);
			last_type = read->d_type;
		}
		read = readdir(current);
		if(read && (last_type == DT_REG || last_type == DT_DIR))
			ret = free_and_join(ret, " ");
	}
	closedir(current);
	return (ret);
}


char *expand(t_lex *lex)
{
	char *ret;

	ret = NULL;
    if (lex->token == ENV)
        ret = ft_strdup(getenv(&lex->content[1]));
    else if (lex->token == STAR)
		ret = handle_star(lex);
	return (ret);
}
