/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 13:11:51 by sonamcrumie       #+#    #+#             */
/*   Updated: 2024/03/25 16:28:21 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static int	ft_negative(int *n)
{
	int	count;

	count = 0;
	if (*n == 0)
		return (ft_print_char('0'));
	if (*n == -2147483648)
	{
		if (write(1, "-2147483648", 11) < 0)
			return (-1);
		*n = 0;
		return (11);
	}
	if (*n < 0)
	{
		if (ft_print_char('-') < 0)
			return (-1);
		*n *= -1;
		count = 1;
	}
	return (count);
}

int	ft_print_int(int n)
{
	int	count;
	int	result;

	count = ft_negative(&n);
	if (count < 0)
		return (-1);
	if (n == 0)
		return (count);
	if (n >= 10)
	{
		result = ft_print_int(n / 10);
		if (result < 0)
			return (-1);
		count += result;
	}
	if (ft_print_char(n % 10 + '0') < 0)
		return (-1);
	return (count + 1);
}
