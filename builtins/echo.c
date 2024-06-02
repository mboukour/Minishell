#include "../minishell.h"


int echo(t_treenode *echo_root)
{
	t_arg *args;
	bool new_line;

	args = echo_root->args;
	new_line = true;
	if(!args->next)
	{
		printf("\n");
		printf("\n");
		return (0);
	}
	args = args->next;
	if(!ft_strcmp(args->content, "-n"))
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
	printf("\n");
	return(0);
}