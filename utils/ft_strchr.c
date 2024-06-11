#include "../minishell.h"




char	*ft_strchr(const char *s, int c)
{
	char	x;
	size_t	i;

	x = (char)c;
	i = 0;
	while (s[i])
	{
		if (s[i] == x)
			return ((char *)&s[i]);
		i++;
	}
	if (x == '\0')
		return ((char *)&s[i]);
	return (NULL);
}