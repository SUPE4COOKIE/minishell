/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ascii_to.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 16:50:30 by scrumier          #+#    #+#             */
/*   Updated: 2024/07/22 16:50:59 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_nondigit(const char **str, int *sign)
{
	*sign = 1;
	if (**str == '\0')
		return ;
	while ((**str >= 9 && **str <= 13) || **str == ' ')
		(*str)++;
	while (**str == '-' || **str == '+')
	{
		if (**str == '-')
			*sign = -(*sign);
		str++;
	}
}

long	ft_atol_overflow(const char *str, bool *is_overflow)
{
	long int	result;
	int			sign;

	result = 0;
	skip_nondigit(&str, &sign);
	while (ft_isdigit(*str))
	{
		if (sign == 1 && (result > (INT_MAX - (*str - '0')) / 10))
		{
			*is_overflow = true;
			return (2);
		}
		if (sign == -1 && (-result < (INT_MIN + (*str - '0')) / 10))
		{
			*is_overflow = true;
			return (2);
		}
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

uint8_t	atoutint8(const char *str)
{
	long int	result;
	bool		is_overflow;

	is_overflow = false;
	result = ft_atol_overflow(str, &is_overflow);
	if (is_overflow == true)
		ft_putendl_fd("Exit : Numeric argument required", 2);
	return (result);
}
