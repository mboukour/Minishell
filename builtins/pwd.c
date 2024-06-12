#include "../minishell.h"

char *get_pwd(void)
{
	char *pwd;
	pwd = getcwd(NULL, 0);
	if(!pwd) // IMPLEMENT BETTER ERROR MANAGEMENT
	{
		perror("Error:");
		return (NULL);
	}
	return (pwd);
}

int pwd(t_treenode *pwd_node, t_data *data)
{
	printf("%s\n", data->pwd);
	return (0);
}