#include "../minishell.h"

int	infooo = 1337;

void sigint_handler_c(int sig)
{
    (void)sig;
	if (infooo == -1)
	{
		close(STDIN_FILENO);
		return ;
	}
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}


char *get_here_doc_input(t_middle *middle)
{
	char *line;
	char *delimiter;
	char *result;
	char *tmp;

	result = NULL;
	delimiter = args_to_str(middle->redirections);
	infooo = -1;
	signal(SIGINT, sigint_handler_c);
	while (true)
	{
		line = readline("> ");
		if(!line)
			break;
		if(!ft_strcmp(line, delimiter))
		{
			free(line);
			break;
		}
		tmp = line;
		line = ft_strjoin(line, "\n", GC);
		free(tmp);
		result = ft_strjoin(result, line, GC);
	}
	infooo = 1337;
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