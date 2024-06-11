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

int pwd(t_treenode *pwd_node)
{
	char *wd;

	wd = get_pwd();
	if(!wd)
		return (1);
	printf("%s\n", wd);
	free(wd);
	return (0);
}