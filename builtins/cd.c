#include "../minishell.h"


void cd(char *path)
{
	if(chdir(path))
		perror("Error");
}


int main()
{
	printf("%s\n", get_pwd());
}