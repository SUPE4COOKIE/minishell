/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:35:17 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/04 19:25:05 by mwojtasi         ###   ########.fr       */
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

int	handle_new_lexer_value(t_lexer *new, char *line, size_t size)
{
	char	*trim;

	if (line[0] == '"' || line[0] == '\'')
		ft_strlcpy(new->value, line + 1, size);
	else
	{
		ft_strlcpy(new->value, line, size + 1);
		trim = new->value;
		new->value = ft_strtrim(new->value, " ");
		free(trim);
		if (new->value == NULL)
			return (1);
	}
	return (0);
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

int	handle_quotes(t_lexer **lex, size_t *start, size_t *end, char *line)
{
	if (*end > *start)
	{
		if (process_word(lex, line, *start, *end))
			return (1);
	}
	*start = *end;
	if (add_quoted_word(lex, line + *start, end))
		return (1);
	*start = *end;
	return (0);
}

int	handle_operator(t_lexer **lex, size_t *start, size_t *end, char *line)
{
	if (*end > *start)
	{
		if (process_word(lex, line, *start, *end))
			return (1);
	}
	*start = *end;
	while (line[*end] && is_operator_char(line[*end]))
	{
		if (add_operator(lex, line + *start, end))
			return (1);
		*start = *end;
	}
	return (0);
}

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
