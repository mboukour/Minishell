#include "../minishell.h"

char *get_pwd(void)
{
	char *pwd;
	pwd = getcwd(NULL, 0);
	if(!pwd) // IMPLEMENT BETTER ERROR MANAGEMENT
		return (NULL);
	return (pwd);
}

int pwd(t_treenode *pwd_node, t_data *data)
{
	char *pwd;

	if (data->pwd)
		printf("%s\n", data->pwd);
	else
	{
		pwd = get_pwd();
		if(!pwd)
		{
			ft_putstr_fd(2, "pwd: error retrieving current directory: getcwd: ");
			perror("");
			return (1);
		}
		printf("%s\n", pwd);
		free(pwd);
	}
	return (0);
}