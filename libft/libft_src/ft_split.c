/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 11:57:11 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/07/13 13:06:15 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static char	*copy_str(const char *str, size_t start, size_t len)
{
	char	*str_copy;
	size_t	i;

	str_copy = malloc(len + 1);
	if (str_copy == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str_copy[i] = str[start + i];
		i++;
	}
	str_copy[i] = 0;
	return (str_copy);
}

static void	free_str(char **split)
{
	size_t	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

static int	fill_remain(size_t i, size_t start, char **s, const char *str)
{
	size_t	j;

	j = 0;
	if (i > start)
	{
		s[j] = copy_str(str, start, i - start);
		if (!s[j])
			return (-1);
		j++;
	}
	return (0);
}

static char	**fill(char **split, char const *str, char sep)
{
	size_t	i;
	size_t	j;
	size_t	start;

	i = 0;
	j = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == (const char)sep)
		{
			if (i > start)
			{
				split[j] = copy_str(str, start, i - start);
				if (!split[j++])
					return (NULL);
			}
			start = i + 1;
		}
		i++;
	}
	if (fill_remain(i, start, &split[j], (char *)str) == -1)
		return (NULL);
	return (split);
}

char	**ft_split(char const *str, char sep)
{
	char	**split;
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	if (str[i] && str[i] != sep)
		count++;
	while (str[i])
	{
		if (str[i] == sep && (str[i + 1] && str[i + 1] != sep))
			count++;
		i++;
	}
	split = malloc(sizeof(char *) * (count + 1));
	if (split == NULL)
		return (NULL);
	split[count] = NULL;
	if (!fill(split, str, sep))
	{
		free_str(split);
		return (NULL);
	}
	return (split);
}
