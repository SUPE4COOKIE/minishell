/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 13:25:06 by sonamcrumie       #+#    #+#             */
/*   Updated: 2024/03/25 16:28:23 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static int	ft_puthex(unsigned long num)
{
	int	count;
	int	tmp;

	count = 0;
	if (num >= 16)
	{
		count += ft_puthex(num / 16);
	}
	tmp = ft_print_char("0123456789abcdef"[num % 16]);
	if (tmp < 0)
		return (-1);
	count += tmp;
	return (count);
}

int	ft_print_ptr(void *ptr)
{
	unsigned long	addr;
	int				count;
	int				tmp;

	count = 0;
	addr = (unsigned long)ptr;
	if (!ptr)
	{
		if (write(1, "(nil)", 5) < 0)
			return (-1);
		return (5);
	}
	if (write(1, "0x", 2) < 0)
		return (-1);
	count += 2;
	tmp = ft_puthex(addr);
	if (tmp < 0)
		return (-1);
	count += tmp;
	return (count);
}
