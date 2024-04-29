/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 03:01:24 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/04/29 04:30:26 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static int	ft_isspace(char c)
{
	if ((c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
			|| c == '\r'))
		return (1);
	return (0);
}

static int	return_overflow(long int result, int nptr, int sign)
{
	if (sign > 0)
	{
		if (result > (INT_MAX - nptr) / 10)
			return (0);
	}
	else
	{
		if (-result < (INT_MIN + nptr) / 10)
			return (-1);
	}
	return (1);
}

int	ft_atoi(const char *nptr)
{
	long int	result;
	int			sign;

	result = 0;
	sign = 1;
	while (ft_isspace(*nptr))
		nptr++;
	if (*nptr == '-')
	{
		sign = -1;
		nptr++;
	}
	else if (*nptr == '+')
		nptr++;
	while (*nptr && ft_isdigit((unsigned char)*nptr))
	{
		if (return_overflow(result, (int)*nptr - '0', sign) != 1)
			return (return_overflow(result, *nptr - '0', sign));
		result = result * 10 + *nptr - '0';
		nptr++;
	}
	return ((int)(result * sign));
}
