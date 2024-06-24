#include "minishell.h"


int main(int ac, char **av, char **env)
{
	int i = 0;
	printf("ARGV: ");
	while(av[i])
	{
		printf("{%s} ", av[i]);
		i++;
	}
	// i = 0;
	// printf("\nENV: ");
	// while(env[i])
	// {
	// 	printf("{%s} ", env[i]);
	// 	i++;
	// }
	printf("\n");
}

