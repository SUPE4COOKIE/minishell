/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_append_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 16:30:53 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/07/13 13:05:45 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	tab_size(char **tab)
{
	size_t	i;

	i = 0;
	if (!tab)
		return (0);
	while (tab[i])
		i++;
	return (i);
}

int	ft_append_str(char ***str, char *add)
{
	char	**new;
	size_t	i;

	new = malloc(sizeof(char *) * (tab_size(*str) + 2));
	if (!new)
		return (1);
	i = 0;
	while ((*str) && (*str)[i])
	{
		new[i] = (*str)[i];
		i++;
	}
	new[i] = ft_strdup(add);
	if (!new[i])
	{
		free(new);
		return (1);
	}
	new[i + 1] = NULL;
	if (*str)
		free(*str);
	*str = new;
	return (0);
}
