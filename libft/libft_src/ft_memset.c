/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:33:15 by scrumier          #+#    #+#             */
/*   Updated: 2024/03/25 16:29:32 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	fill;
	size_t			i;

	str = s;
	fill = c;
	i = 0;
	while (n > 0)
	{
		str[i] = fill;
		i++;
		n--;
	}
	return (s);
}
