/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_delete_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 19:15:28 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/10 21:48:51 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_lexer(t_lexer *lex)
{
	t_lexer	*tmp;

	while (lex)
	{
		tmp = lex;
		lex = lex->next;
		if (tmp->value)
			free(tmp->value);
		if (tmp)
			free(tmp);
	}
}

t_lexer	*delete_lexer(t_lexer **lex, t_lexer *to_delete)
{
	t_lexer	*tmp;

	tmp = *lex;
	if (tmp == to_delete)
	{
		*lex = tmp->next;
		free_null(tmp->value);
		free_null(tmp);
		return (NULL);
	}
	while (tmp && tmp != to_delete)
		tmp = tmp->next;
	if (tmp && tmp == to_delete)
	{
		if (tmp->prev)
		{
			tmp->prev->next = tmp->next;
			if (!tmp->prev->space_after)
				tmp->prev->space_after = tmp->space_after;
		}
		if (tmp->next)
			tmp->next->prev = tmp->prev;
		free(tmp->value);
	}
	return (free(tmp), *lex);
}
