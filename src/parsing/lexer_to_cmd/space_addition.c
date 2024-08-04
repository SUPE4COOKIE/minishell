/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   space_addition.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 16:57:29 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/04 16:57:40 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_nospace_addable(t_token_type type)
{
	if (type == T_WORD || type == T_S_QUOTED_WORD || type == T_D_QUOTED_WORD)
		return (true);
	return (false);
}

int	nospace_add(char **args, t_lexer **lex)
{
	size_t	size;
	t_lexer *start;

	size = 0;
	start = *lex;
	while (*lex && (*lex)->value && (!size || (!(*lex)->prev->space_after)) && is_nospace_addable((*lex)->type))
	{
		size += ft_strlen((*lex)->value) + 1;
		*lex = (*lex)->next;
	}
	*lex = start;
	*args = ft_calloc(size + 1, sizeof(char));
	if (!*args)
		return (-1);
	while (*lex && (*lex)->value && !(*lex)->space_after && is_nospace_addable((*lex)->type))
	{
		ft_strlcat(*args, (*lex)->value, size + 1);
		*lex = (*lex)->next;
	}
	if (*lex && (*lex)->value)
		ft_strlcat(*args, (*lex)->value, size + 1);
	return (0);
}