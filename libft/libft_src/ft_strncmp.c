/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 15:30:41 by scrumier          #+#    #+#             */
/*   Updated: 2024/03/25 16:30:10 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	c;

	c = 0;
	if (n == 0)
		return (0);
	if (s1 == NULL || s2 == NULL || *s1 == '\0' || *s2 == '\0')
		return (-1);
	while (s1[c] && s2[c] && s1[c] == s2[c] && c < n - 1)
		c++;
	return (((unsigned char)(s1[c]) - (unsigned char)(s2[c])));
}
