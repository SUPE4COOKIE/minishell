/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:35:17 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/07 03:35:36 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lexer_director(t_lexer **lex, size_t *end, size_t *start, char *line)
{
	while (line[*end])
	{
		if (line[*end] == '"' || line[*end] == '\'')
		{
			if (handle_quotes(lex, start, end, line))
				return (1);
			continue ;
		}
		if (is_operator_char(line[*end]))
		{
			if (handle_operator(lex, start, end, line))
				return (1);
			continue ;
		}
		(*end)++;
	}
	return (0);
}

int	finalize_lexer(t_lexer **lex, char *line, size_t start, size_t end)
{
	if (end > start)
	{
		if (get_lexer_type(line + start) == T_WORD)
		{
			if (split_word_lexer(lex, line + start, end - start))
				return (1);
		}
		else
		{
			if (new_lexer(lex, line + start, end - start))
				return (1);
			get_last_lexer(*lex)->space_after = false;
		}
	}
	return (0);
}

t_lexer	*lexer(char *line)
{
	t_lexer	*lex;
	size_t	end;
	size_t	start;

	end = 0;
	start = 0;
	lex = NULL;
	if (line[end] && lexer_director(&lex, &end, &start, line))
	{
		free_lexer(lex);
		return (NULL);
	}
	if (finalize_lexer(&lex, line, start, end))
	{
		free_lexer(lex);
		return (NULL);
	}
	return (lex);
}
