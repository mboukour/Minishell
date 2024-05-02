#include "../minishell.h"


void ft_putstr_fd(int fd, char *str)
{
	int i;

	i = 0;
	while(str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
}
