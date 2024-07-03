#include <stdio.h>




int main(char ac, char **av, char **envp)
{
   int i = 0;

//    while(av[i])
//    {
// 		printf("{%s} ", av[i]);
// 		i++;
//    }
   i = 0;
   while (envp[i])
   {
		printf("%s\n", envp[i]);
		i++;
   }

}