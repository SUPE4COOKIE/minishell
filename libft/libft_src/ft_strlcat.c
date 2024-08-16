/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 15:30:49 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/16 11:48:14 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	dest_size;
	size_t	src_size;

	dest_size = 0;
	src_size = 0;
	i = 0;
	while (dest[dest_size] && dest_size < size)
		dest_size++;
	while (src[src_size])
		src_size++;
	while (src[i] && (dest_size + i + 1 < size))
	{
		dest[dest_size + i] = src[i];
		i++;
	}
	if (dest_size < size)
		dest[dest_size + i] = '\0';
	return (dest_size + src_size);
}
