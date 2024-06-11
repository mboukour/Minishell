#include "../minishell.h"


int cd_core(char *path)
{
	if(chdir(path))
	{
		ft_putstr_fd(2, "CD Error\n");
		return (1);
	}
	return (0);
}

int cd(t_treenode *cd_root)
{
	t_arg *args;

	args = cd_root->args;
	if(!args)
		return (cd_core(""));
	else if (!args->next)
		return (cd_core(args->content));
	ft_putstr_fd(2, "cd: too many arguments\n");
	return (1);
}