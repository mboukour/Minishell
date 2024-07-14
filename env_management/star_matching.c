/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   star_matching.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 02:25:10 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/14 02:27:06 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_management.h"

t_env	*get_star(int mode)
{
	DIR				*current_wd;
	struct dirent	*read;
	t_env			*current;
	t_env			*head;

	current = NULL;
	head = NULL;
	current_wd = opendir(".");
	if (!current_wd)
		return (NULL);
	read = readdir(current_wd);
	while (read != NULL)
	{
		if ((read->d_type == DT_REG || read->d_type == DT_DIR)
			&& !(mode == SEEN && read->d_name[0] == '.'))
		{
			current = ft_lstnew_env(read->d_name, GC);
			ft_lstadd_back_env(&head, current);
		}
		read = readdir(current_wd);
	}
	return (closedir(current_wd), (head));
}

char	*normalize_pattern(char *pattern)
{
	char *normalized;
	char *start;
	bool star_found = false;
	int pattern_len = ft_strlen(pattern);
	int i = 0;
	int j = 0;

	normalized = smart_malloc(pattern_len + 1);
	start = normalized;
	while (pattern[i])
	{
		if (pattern[i] == '*')
		{
			if (!star_found)
			{
				normalized[j] = pattern[i];
				j++;
				star_found = true;
			}
		}
		else
		{
			normalized[j] = pattern[i];
			j++;
			star_found = false;
		}
		i++;
	}
	normalized[j] = '\0';
	return (start);
}

bool	match_pattern(char *str, char *pattern)
{
	char	*star_ptr;
	char	*star_str;
	int		str_index;
	int		pattern_index;

	star_ptr = NULL;
	star_str = NULL;
	str_index = 0;
	pattern_index = 0;
	while (str[str_index])
	{
		if (pattern[pattern_index] == '*')
		{
			star_ptr = &pattern[pattern_index];
			star_str = &str[str_index];
			pattern_index++;
		}
		else if (pattern[pattern_index] == str[str_index])
		{
			pattern_index++;
			str_index++;
		}
		else if (star_ptr)
		{
			pattern_index = star_ptr - pattern + 1;
			str_index = star_str - str + 1;
			star_str++;
		}
		else
			return (false);
	}
	while (pattern[pattern_index] == '*')
		pattern_index++;
	return !pattern[pattern_index];
}

bool	is_star(char *to_match)
{
	int	i;

	i = 0;
	while (to_match[i])
	{
		if (to_match[i] == '*')
			return (true);
		i++;
	}
	return (false);
}

t_env	*star_matching(char *to_match)
{
	t_env	*current_star;
	t_env	*head;
	t_env	*current;
	char	*normal;

	head = NULL;
	if (to_match[0] == '.')
		current_star = get_star(HIDDEN);
	else
		current_star = get_star(SEEN);
	if (!current_star)
		return (NULL);
	normal = normalize_pattern(to_match);
	while (current_star)
	{
		if (current_star->value && match_pattern(current_star->value, normal))
		{
			current = ft_lstnew_env(current_star->value, GC);
			ft_lstadd_back_env(&head, current);
		}
		current_star = current_star->next;
	}
	return (sort_env_list(head), head);
}
