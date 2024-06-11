#include "../minishell.h"


int exit_core(int status, t_env *env)
{
	ft_lstclear_env(env);
	smart_free();
	exit(status);
	return (0);
}

bool is_num(char *str)
{
	int i;

	i = 0;
	while(str[i])
	{
		if(str[i] > '9' || str[i] < '0')
			return (false);
		i++;
	}
	return (true);
}

int exit_cmd(t_treenode *root, t_env *env)
{
	t_arg *args;

	args = root->args;
	args = args->next;
	ft_putstr_fd(1, "exit\n");
	if(!args)
		exit_core(0, env);
	else if(!is_num(args->content))
	{
		ft_putstr_fd(2,"exit: ");
		ft_putstr_fd(2,args->content);
		ft_putstr_fd(2, " numeric argument required\n");
		exit_core(2, env);
		return (1);
	}
	else if(args->next)
	{
		ft_putstr_fd(2,"exit: too many arguments\n");
		exit_core(1, env);
		return (1);
	}
	exit_core(ft_atoi(args->content), env);
}