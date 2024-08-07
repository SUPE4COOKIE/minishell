/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:35:58 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/07 16:37:27 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexer	*lexer_replacer(t_lexer *lex, char *value, t_lexer **origin)
{
	t_lexer	*tmp;
	t_lexer	*before;
	t_lexer	*after;
	t_lexer	*last;

	tmp = lexer(value);
	if (!tmp)
		return (NULL);
	before = lex->prev;
	after = lex->next;
	if (before)
		before->next = tmp;
	tmp->prev = before;
	last = get_last_lexer(tmp);
	last->next = after;
	last->space_after = lex->space_after;
	if (after)
		after->prev = last;
	free_null(lex);
	if (!before)
		*origin = tmp;
	return (last);
}

bool	contain_spaced_words(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] && ft_iswhitespace(str[i]))
		i++;
	if (!str[i])
		return (false);
	while (str[i] && !ft_iswhitespace(str[i]))
		i++;
	if (!str[i])
		return (false);
	while (str[i] && ft_iswhitespace(str[i]))
		i++;
	if (!str[i])
		return (false);
	return (true);
}