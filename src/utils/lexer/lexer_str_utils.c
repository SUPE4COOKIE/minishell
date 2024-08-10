/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_str_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 18:58:50 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/10 21:49:24 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
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

int	is_n_only_spaces(char *line, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n'
			&& line[i] != '\v' && line[i] != '\f' && line[i] != '\r')
			return (0);
		i++;
	}
	return (1);
}

size_t	skip_whitespaces(char *line, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size && ft_iswhitespace(line[i]))
		i++;
	return (i);
}
