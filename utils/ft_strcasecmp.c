#include "../minishell.h"


int	ft_toupper(int c)
{
	if (c >= 97 && c <= 122)
		return (c - 32);
	return (c);
}


int ft_strcasecmp(const char *s1, const char *s2) 
{
	char c1;
	char c2;

    while (*s1 && *s2) 
	{
        c1 = ft_toupper(*s1);
        c2 = ft_toupper(*s2);
        if (c1 != c2)
            return c1 - c2;
        s2++;
        s1++;
    }
    return (ft_toupper(*s1) - ft_toupper(*s2));
}
