/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 16:10:55 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/04/29 04:30:26 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static size_t	is_trim_char(const char c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*result;
	size_t	i;
	size_t	start;
	size_t	end;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	end = ft_strlen((char *)s1);
	while (s1[start] && is_trim_char(s1[start], set))
		start++;
	while (end > start && is_trim_char(s1[end - 1], set))
		end--;
	result = malloc(end - start + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (start < end)
		result[i++] = s1[start++];
	result[i] = '\0';
	return (result);
}
