/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 10:15:44 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/16 11:48:21 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*str;
	int		i;
	size_t	j;

	i = -1;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	str = (char *)malloc(
			sizeof(*s1) * (ft_strlen((char *)s1) + ft_strlen((char *)s2) + 1));
	if (!str)
		return (NULL);
	while (s1[++i])
		str[j++] = s1[i];
	i = -1;
	while (s2[++i])
		str[j++] = s2[i];
	str[j] = 0;
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	return (str);
}
