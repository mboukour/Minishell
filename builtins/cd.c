#include "../minishell.h"


void cd(char *path)
{
	if(chdir(path))
		perror("Error");
}


int main()
{
	cd("..");
	//printf("%s\n", get_pwd());
	system("pwd");
}