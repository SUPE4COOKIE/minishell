/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:10:05 by scrumier          #+#    #+#             */
/*   Updated: 2024/03/25 16:28:58 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

long	ft_atoi(const char *nptr)
{
	long int	result;
	int			sign;

	sign = 1;
	result = 0;
	if (*nptr == '\0')
		return (0);
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == ' ')
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while (ft_isdigit(*nptr))
	{
		if (sign == 1 && (result > (INT_MAX - (*nptr - '0')) / 10))
			return (LONG_MAX - 1);
		if (sign == -1 && (-result < (INT_MIN + (*nptr - '0')) / 10))
			return (LONG_MIN + 1);
		result = result * 10 + (*nptr - '0');
		nptr++;
	}
	return (result * sign);
}
