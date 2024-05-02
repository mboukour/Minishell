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

