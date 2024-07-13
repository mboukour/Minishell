/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 05:20:14 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 05:20:39 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void init_t_data(t_data *data)
{
    data->infile = -1;
    data->outfile = -1;
    data->end[0] = -1;
    data->end[1] = -1;
    data->status = 0;
    data->cmd = NULL;
    data->path = NULL;
    data->env = NULL;
    data->pwd = getcwd(NULL, 0);
    if(!data->pwd)
    {
        ft_putstr_fd(2, "error retrieving current directory: getcwd: cannot access parent directories: ");
        perror("");
    }
    data->old_pwd = NULL;
}
bool is_all_space(char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        if(!is_ws(str[i]))
            return (false);
        i++;
    }
    return (true);
}
