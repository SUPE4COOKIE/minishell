/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_modify_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 19:18:48 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/07 03:40:01 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	new_lexer(t_lexer **lex, char *line, size_t size)
{
	t_lexer	*new;

	if (!line || !line[0])
		return (0);
	if (is_n_only_spaces(line, size))
		return (0);
	new = malloc(sizeof(t_lexer));
	if (new == NULL)
		return (1);
	new->value = malloc(size + 1);
	if (new->value == NULL)
		return (free(new), 1);
	if (handle_new_lexer_value(new, line, size))
	{
		free(new->value);
		free(new);
		return (1);
	}
	new->type = get_lexer_type(line);
	append_new_lexer(lex, &new);
	new->next = NULL;
	new->space_after = false;
	return (0);
}

int	add_operator(t_lexer **lex, char *line, size_t *end)
{
	size_t	len;

	len = 0;
	while (line[len] && is_operator_char(line[len])
		&& line[0] == line[len] && len < 2)
	{
		len++;
		if (line[0] == '|')
			break ;
	}
	if (new_lexer(lex, line, len))
		return (1);
	(*end) += len;
	get_last_lexer(*lex)->space_after = ft_iswhitespace(line[len]);
	return (0);
}


int	add_quoted_word(t_lexer **lex, char *line, size_t *end)
{
	size_t	len;
	t_lexer	*tmp;
	char	quote;

	quote = line[0];
	len = 1;
	while (line[len] && line[len] != quote)
		len++;
	if (len > 1)
	{
		if (new_lexer(lex, line, len)) //TODO: check return value
			return (1);
	}
	if (line[len] == quote)
		len++;
	(*end) += len;
	tmp = get_last_lexer(*lex);
	if (ft_iswhitespace(line[len]) && tmp)
		tmp->space_after = true;
	else if (tmp && !tmp->space_after)
		tmp->space_after = false;
	return (0);
}

void	append_new_lexer(t_lexer **lex, t_lexer **new)
{
	t_lexer	*tmp;

	if (*lex == NULL)
	{
		*lex = *new;
		(*lex)->prev = NULL;
	}
	else
	{
		tmp = *lex;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = *new;
		(*new)->prev = tmp;
	}
}