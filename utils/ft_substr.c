#include "../minishell.h"


char	*ft_substr(const char *s, unsigned int start, size_t len, int mode)
{
	size_t	i;
	char	*substr;

	i = 0;
	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup("", GC));
	if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	if(mode == MANUAL)
	{
		substr = malloc((len + 1) * sizeof(char));
		if(!substr)
			return(NULL);
	}
	else
		substr = smart_malloc((len + 1) * sizeof(char));
	while (i < len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}
