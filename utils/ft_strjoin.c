#include "../minishell.h"



char	*ft_strjoin(const char *s1, const char *s2, int mode)
{
	char	*finalstr;
	int		i;
	int		j;

	if (!s1 && s2)
		return (ft_strdup(s2, mode));
	else if (s1 && !s2)
		return (ft_strdup(s1, mode));
	else if (!s1 && !s2)
		return (ft_strdup("", mode));
	i = 0;
	j = 0;
	if(mode == MANUAL)
	{
		finalstr = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1)
			* sizeof(char));
		if(!finalstr)
			return(NULL);
	}
	else
		finalstr = (char *)smart_malloc((ft_strlen(s1) + ft_strlen(s2) + 1)
				* sizeof(char));
	while (s1[i])
		finalstr[j++] = s1[i++];
	i = 0;
	while (s2[i])
	{
		finalstr[j] = s2[i];
		j++;
		i++;
	}
	finalstr[j] = 0;
	return (finalstr);
}