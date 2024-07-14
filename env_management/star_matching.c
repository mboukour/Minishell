/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   star_matching.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 02:25:10 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/14 04:50:41 by mboukour         ###   ########.fr       */
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

void	skip_stars(char *pattern, int *pt_index)
{
	while (pattern[*pt_index] == '*')
		(*pt_index)++;
}

bool	match_pattern(char *to_match, char *pattern, int tm_index, int pt_index)
{
	int	last_star_index;

	last_star_index = NONE;
	while (to_match[tm_index])
	{
		if (pattern[pt_index] == '*')
		{
			last_star_index = pt_index;
			pt_index++;
		}
		else if (to_match[tm_index] == pattern[pt_index])
		{
			tm_index++;
			pt_index++;
		}
		else if (last_star_index != NONE)
		{
			pt_index = last_star_index + 1;
			tm_index++;
		}
		else
			return (false);
	}
	return (skip_stars(pattern, &pt_index), !pattern[pt_index]);
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
	while (current_star)
	{
		if (current_star->value && match_pattern(current_star->value,
				to_match, 0, 0))
		{
			current = ft_lstnew_env(current_star->value, GC);
			ft_lstadd_back_env(&head, current);
		}
		current_star = current_star->next;
	}
	return (sort_env_list(head), head);
}
