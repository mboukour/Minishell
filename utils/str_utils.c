/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 03:59:07 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 04:08:00 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_toupper(int c)
{
	if (c >= 97 && c <= 122)
		return (c - 32);
	return (c);
}

int	ft_strcasecmp(const char *s1, const char *s2)
{
	char	c1;
	char	c2;

	while (*s1 && *s2)
	{
		c1 = ft_toupper(*s1);
		c2 = ft_toupper(*s2);
		if (c1 != c2)
			return (c1 - c2);
		s2++;
		s1++;
	}
	return (ft_toupper(*s1) - ft_toupper(*s2));
}

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

char	**malloced_for_split(int size, int mode)
{
	char	**all_words;

	if (mode == MANUAL)
	{
		all_words = malloc(sizeof(char *) * (size));
		if (!all_words)
			return (NULL);
	}
	else
		all_words = (char **)smart_malloc(sizeof(char *) * (size));
	return (all_words);
}
