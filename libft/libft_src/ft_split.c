/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:33:28 by scrumier          #+#    #+#             */
/*   Updated: 2024/03/25 16:29:47 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static char	**ft_free_array(char **array, size_t nb_wrd)
{
	char	**current;

	current = array;
	while (nb_wrd--)
	{
		free(*current);
		*current = NULL;
		current++;
	}
	free(array);
	return (NULL);
}

static size_t	ft_count_wrds(char const *s, char c)
{
	size_t	nb_wrd;
	size_t	sep_wrd;

	sep_wrd = 0;
	nb_wrd = 0;
	while (*s)
	{
		if (*s != c && !sep_wrd)
		{
			sep_wrd = 1;
			nb_wrd++;
		}
		else if (*s == c && sep_wrd)
			sep_wrd = 0;
		s++;
	}
	return (nb_wrd);
}

static size_t	ft_next_word_len(const char *s, char c)
{
	size_t	len;

	len = 0;
	while (*s && *s == c)
		s++;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

char	**ft_split(char const *s, char c)
{
	size_t	len;
	char	**list;
	size_t	i;

	if (!s)
		return (NULL);
	list = ft_calloc(ft_count_wrds(s, c), sizeof(char *));
	if (!list)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s)
		{
			len = ft_next_word_len(s, c);
			list[i++] = ft_substr(s, 0, len);
			if (list[i - 1] == NULL)
				return (ft_free_array(list, i - 1));
			s += len;
		}
	}
	return (list);
}
