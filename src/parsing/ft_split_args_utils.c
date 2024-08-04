/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_args_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 17:50:34 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/04 11:15:10 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**init_split(size_t count)
{
	char	**split;

	split = malloc(sizeof(char *) * (count + 1));
	if (split == NULL)
		return (NULL);
	split[count] = NULL;
	return (split);
}

void	free_str(char **split)
{
	size_t	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free_null(split[i]);
		i++;
	}
	free_null(split);
}

int	is_in_quote(char c, char *in_quote)
{
	if (c == '\'' || c == '"')
	{
		if (*in_quote == 0)
			*in_quote = c;
		else if (*in_quote == c)
			*in_quote = 0;
		return (1);
	}
	return (0);
}

char	*copy_str(const char *str, size_t start, size_t len)
{
	char	*str_copy;
	size_t	i;
	size_t	j;

	str_copy = malloc(len + 1);
	if (str_copy == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (str[start + i] != '\'' && str[start + i] != '"')
		{
			str_copy[j] = str[start + i];
			j++;
		}
		i++;
	}
	str_copy[j] = '\0';
	return (str_copy);
}
