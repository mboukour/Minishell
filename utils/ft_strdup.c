#include "../minishell.h"


char	*ft_strdup(const char *s1, int mode)
{
	char	*copy;
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(s1);
	if(mode == MANUAL)
	{
		copy = (char *)malloc(sizeof(char) * (len + 1));
		if(!copy)
			return (NULL);
	}
	else
		copy = (char *)smart_malloc(sizeof(char) * (len + 1));
	while (i < len)
	{
		copy[i] = s1[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}


