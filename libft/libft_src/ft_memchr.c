/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:33:02 by scrumier          #+#    #+#             */
/*   Updated: 2024/03/25 16:29:23 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	char	*s_tmp;
	size_t	i;

	s_tmp = (char *)s;
	i = 0;
	while (i < n)
	{
		if (s_tmp[i] == (char)c)
			return (&s_tmp[i]);
		i++;
	}
	return (0);
}
