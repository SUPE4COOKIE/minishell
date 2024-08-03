/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 11:57:11 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/03 13:20:40 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	fill_node(size_t index[2], size_t *start, char **split, char const *str)
{
	if (index[0] > *start)
	{
		split[index[1]] = copy_str(str, *start, index[0] - *start);
		if (!split[index[1]++])
			return ;
	}
	*start = index[0] + 1;
}

static char	**fill(char **split, char const *str, char sep)
{
	size_t	index[2];
	size_t	start;
	char	in_quote;

	index[0] = 0;
	index[1] = 0;
	start = 0;
	in_quote = 0;
	while (str[index[0]])
	{
		is_in_quote(str[index[0]], &in_quote);
		if (str[index[0]] == (const char)sep && !in_quote)
			fill_node(index, &start, split, str);
		index[0]++;
	}
	if (fill_remain(index[0], start, &split[index[1]], (char *)str) == -1)
		return (NULL);
	return (split);
}

char	**ft_split_args(char const *str, char sep)
{
	char	**split;
	size_t	count;
	size_t	i;
	char	in_quote;

	count = 0;
	i = 0;
	in_quote = 0;
	if (str[i] && str[i] != sep)
		count++;
	while (str[i])
	{
		is_in_quote(str[i], &in_quote);
		if (str[i] == sep && (str[i + 1] && str[i + 1] != sep) && !in_quote)
			count++;
		i++;
	}
	split = init_split(count);
	if (!split || !fill(split, str, sep))
	{
		free_str(split);
		return (NULL);
	}
	return (split);
}
