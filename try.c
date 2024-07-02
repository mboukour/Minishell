#include <stdio.h>




int main(char ac, char **av)
{
   int i = 0;

   while(av[i])
   {
		printf("{%s} ", av[i]);
		i++;
   }
   printf("\n");
}