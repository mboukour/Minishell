/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fun_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabrouk <amabrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 06:28:00 by amabrouk          #+#    #+#             */
/*   Updated: 2024/05/25 15:31:00 by amabrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline.h"

int	word_count(char *s)
{
	int	wc;
	int	i;

	i = 0;
	wc = 0;
	while (s[i])
	{
		while (s[i] && (s[i] == ' ' || s[i] == '\t'))
			i++;
		if (s[i] && (s[i] != ' ' && s[i] != '\t'))
			wc++;
		while (s[i] && (s[i] != ' ' && s[i] != '\t'))
			i++;
	}
	return (wc);
}

char	*ft_fill(char *s, int *i)
{
	char	*copy;
	int		start;
	int		k;

	while (s[*i] && (s[*i] == ' ' || s[*i] == '\t'))
		(*i)++;
	start = *i;
	while (s[*i] && s[*i] != ' ' && s[*i] != '\t')
		(*i)++;
	copy = malloc(sizeof(char) * (*i - start) + 1);
	if (!copy)
		return (NULL);
	k = 0;
	while (*i > start)
		copy[k++] = s[start++];
	copy[k] = 0;
	return (copy);
}

void	ft_free(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}

char	**split_option(char *s)
{
	char	**res;
	int		i;
	int		k;
	int		wc;

	if (!s)
		return (NULL);
	wc = word_count(s);
	res = (char **)malloc(sizeof(char *) * (wc + 1));
	if (!res)
		return (NULL);
	k = 0;
	i = 0;
	while (k < wc)
	{
		res[k] = ft_fill(s, &i);
		if (!res[k])
			return (ft_free(res), NULL);
		k++;
	}
	res[k] = 0;
	return (res);
}
