#include "../minishell.h"

bool is_all_n(char *content)
{
	int i;

	i = 0;
	while(content[i] && content[i] == 'n')
		i++;
	if(content[i])
		return (false);
	return (true);
}

int echo(t_treenode *echo_root)
{
	t_arg *args;
	bool new_line;

	args = echo_root->args;
	new_line = true;
	if(!args)
	{
		fprintf(stderr, "No args\n");
		printf("\n");
		return (0);
	}
	// args = args->next;
	while(args && !ft_strncmp(args->content, "-n", 2) && is_all_n(args->content + 2))
	{
		new_line = false;
		args = args->next;
	}
	while(args)
	{
		printf("%s", args->content);
		if(args->next)
			printf(" ");
		args = args->next;
	}
	if(new_line)
		printf("\n");
	return(0);
}