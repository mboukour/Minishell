#include "../minishell.h"



void env(char **envp)
{
	while(*envp)
		printf("%s\n", *envp++);
}

int main(int ac, char **av, char **envp)
{
	env(envp);
}