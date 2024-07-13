/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 03:58:17 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 03:58:29 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_malloced(int mode, const char *s1, const char *s2)
{
	char	*finalstr;

	if (mode == MANUAL)
	{
		finalstr = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1)
				* sizeof(char));
		if (!finalstr)
			return (NULL);
	}
	else
		finalstr = (char *)smart_malloc((ft_strlen(s1) + ft_strlen(s2) + 1)
				* sizeof(char));
	return (finalstr);
}

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
	finalstr = get_malloced(mode, s1, s2);
	if (mode == MANUAL && !finalstr)
		return (NULL);
	while (s1[i])
		finalstr[j++] = s1[i++];
	i = 0;
	while (s2[i])
		finalstr[j++] = s2[i++];
	finalstr[j] = 0;
	return (finalstr);
}
