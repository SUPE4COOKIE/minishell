/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ascii_to.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 16:50:30 by scrumier          #+#    #+#             */
/*   Updated: 2024/07/23 14:34:38 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_nondigit(const char *str, int *sign, int *i)
{
	*sign = 1;
	*i = 0;
	if (str[*i] == '\0')
		return (1);
	while ((str[*i] >= 9 && str[*i] <= 13) || str[*i] == ' ')
		(*i)++;
	while (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			*sign *= -1;
		(*i)++;
	}
	return (0);
}

long long	ft_atol_overflow(const char *str, bool *is_overflow)
{
	long long	result;
	int			sign;
	int			i;

	i = 0;
	result = 0;
	sign = 1;
	if (skip_nondigit(str, &sign, &i) == 1)
		return (2);
	while (ft_isdigit(str[i]) == 1)
	{
		if (sign == 1 && (result > (LONG_MAX - (str[i] - '0')) / 10))
		{
			*is_overflow = true;
			return (2);
		}
		if (sign == -1 && (-result < (LONG_MIN + (str[i] - '0')) / 10))
		{
			*is_overflow = true;
			return (2);
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

uint8_t	atoutint8(const char *str, bool *is_overflow)
{
	uint8_t		result;

	result = 0;
	*is_overflow = false;
	result += ft_atol_overflow(str, is_overflow);
	return (result);
}
