/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 03:24:48 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/07 03:35:38 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	split_remaining(t_lexer **lex, char *line, size_t size)
{
	if (size <= 0)
		return (0);
	if (new_lexer(lex, line, size))
		return (1);
	if (ft_iswhitespace(line[size - 1]) && *lex)
		get_last_lexer(*lex)->space_after = true;
	else if (*lex)
		get_last_lexer(*lex)->space_after = false;
	return (0);
}

int	split_word_lexer(t_lexer **lex, char *line, size_t size)
{
	size_t	i;
	int		error_code;

	i = skip_whitespaces(line, size);
	while (i < size)
	{
		if (ft_iswhitespace(line[i]))
		{
			if (i > 0)
			{
				error_code = new_lexer(lex, line, i);
				if (error_code)
					return (error_code);
				if (!(*lex))
					return (0);
				get_last_lexer(*lex)->space_after = true;
			}
			line += i + 1;
			size -= i + 1;
			i = 0;
		}
		i++;
	}
	return (split_remaining(lex, line, size));
}
int	process_word(t_lexer **lex, char *line, size_t start, size_t end)
{
	int	type;

	type = get_lexer_type(line + start);
	if (type == T_WORD)
	{
		if (split_word_lexer(lex, line + start, end - start))
			return (1);
	}
	else
	{
		if (new_lexer(lex, line + start, end - start))
			return (1);
		get_last_lexer(*lex)->space_after = ft_iswhitespace(line[end]);
	}
	return (0);
}