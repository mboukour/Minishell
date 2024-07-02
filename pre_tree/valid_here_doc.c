#include "../minishell.h"

char *get_here_doc_input(t_middle *middle)
{
	char *line;
	char *delimiter;
	char *result;
	char *tmp;

	result = NULL;
	delimiter = args_to_str(middle->redirections);
	while (true)
	{
		line = readline("> ");
		if(!line || !ft_strcmp(line, delimiter))
			break;
		tmp = line;
		line = ft_strjoin(line, "\n", GC);
		free(tmp);
		result = ft_strjoin(result, line, GC);
	}
	return (result);
	
}


void valid_here_doc(t_middle *middle)
{
	while(middle)
	{
		if(middle->token == HERE_DOC)
			middle->here_doc_buffer = get_here_doc_input(middle);
		middle = middle->next;
	}
}