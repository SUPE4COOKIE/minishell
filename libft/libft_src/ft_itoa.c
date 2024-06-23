/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:32:31 by scrumier          #+#    #+#             */
/*   Updated: 2024/06/23 00:13:42 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static size_t	int_size(long int n)
{
	size_t	count;

	count = 1;
	if (n < 0)
		count++;
	while ((n / 10))
	{
		count++;
		n /= 10;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	long int	nb;
	char		*str;
	size_t		i;

	nb = (long int)n;
	i = int_size(nb);
	str = malloc((i + 1));
	if (!str)
		return (NULL);
	str[i] = '\0';
	if (nb < 0)
	{
		str[0] = '-';
		nb *= -1;
	}
	if (nb == 0)
		str[0] = '0';
	while (nb)
	{
		str[--i] = (char)((nb % 10) + '0');
		nb /= 10;
	}
	return (str);
}
