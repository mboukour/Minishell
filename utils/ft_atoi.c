/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 04:04:59 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/13 04:05:07 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	set_error(bool *error_status)
{
	if (error_status)
		*error_status = true;
}


static int	str_to_int(const char *str, int i, int sign, bool *error_status)
{
	unsigned long long	nb;
	int					count;

	nb = 0;
	count = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		count++;
		if (count == 20)
		{
			set_error(error_status);
			if (sign == -1)
				return (0);
			return (-1);
		}
		nb = nb * 10 + str[i] - '0';
		i++;
	}
	if (nb > LONG_MAX)
	{
		if (sign == -1)
			return (set_error(error_status), 0);
		return (set_error(error_status), -1);
	}
	return ((int)nb * sign);
}

int	ft_atoi(const char *str, bool *error_status)
{
	int		sign;
	size_t	i;

	sign = 1;
	i = 0;
	while (str[i] && is_ws(str[i] || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] == '0')
		i++;
	return (str_to_int(str, i, sign, error_status));
}
