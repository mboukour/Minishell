#include "minishell.h"



int main(char ac, char **av, char **envp)
{
   int i = 0;
 	signal(SIGINT, SIG_IGN);
	while(1)
	{
		printf("y\n");
	}
//    while(av[i])
//    {
// 		printf("{%s} ", av[i]);
// 		i++;
//    }
//    printf("\n");
//    i = 0;
//    while (envp[i])
//    {
// 		printf("%s\n", envp[i]);
// 		i++;
//    }
	// getcwd(NULL, 0);
}