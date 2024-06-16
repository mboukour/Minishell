#include "../minishell.h"

size_t ft_strncpy(char *dst, const char *src, size_t dstsize)
{
    size_t i;

	i = 0;
    while (i < dstsize - 1 && src[i])
    {
        dst[i] = src[i];
        i++;
    }
    if (i < dstsize)
        dst[i] = '\0';
    return i;
}