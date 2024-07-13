/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 04:00:33 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 04:00:35 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strdup(const char *s1, int mode)
{
	char	*copy;
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(s1);
	if (mode == MANUAL)
	{
		copy = (char *)malloc(sizeof(char) * (len + 1));
		if (!copy)
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

size_t	ft_strcpy(char *dst, const char *src)
{
	size_t	len;
	size_t	i;

	len = ft_strlen(src);
	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (len);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

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
