/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 02:25:32 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/07 02:26:38 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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