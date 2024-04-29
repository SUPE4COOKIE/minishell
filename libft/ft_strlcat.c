/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 03:50:22 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/04/29 04:30:26 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

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

/*#include <stdio.h>
int	main(void)
{
	char dest[20] = "voici un ";
	char src[10] = "testdsf";

	printf("%d %s",ft_strlcat(dest, src, 11),dest);

}*/
