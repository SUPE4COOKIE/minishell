/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_uint.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 13:14:31 by sonamcrumie       #+#    #+#             */
/*   Updated: 2024/03/25 16:28:27 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

int	ft_print_uint(unsigned int num)
{
	int	count;
	int	tmp;

	count = 0;
	if (num >= 10)
	{
		tmp = ft_print_uint(num / 10);
		if (tmp == -1)
			return (-1);
		count += tmp;
	}
	if (ft_print_char(num % 10 + '0') < 0)
		return (-1);
	count++;
	return (count);
}
