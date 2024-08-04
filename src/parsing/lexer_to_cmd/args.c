/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 16:56:17 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/04 17:01:28 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	allocate_args_cmd(t_lexer *lex, char ***args, char ***args_start, t_cmd **last_cmd)
{
	int	word_num;

	word_num = get_word_num(lex);
	*args = ft_calloc(word_num + 1, sizeof(char *));
	*args_start = *args;
	if (!*args)
		return (-1);
	*last_cmd = new_cmd(NULL);
    if (!*last_cmd)
        return (free(*args), -1);
	return (0);
}

int	handle_args(char ***args, t_lexer **lex)
{
	if (!(*lex)->space_after && (*lex)->next &&
		is_nospace_addable((*lex)->next->type))
	{
		if (nospace_add(*args, lex))
			return (-1);
	}
	else
	{
		**args = ft_strdup((*lex)->value);
		if (!**args)
			return (-1);
	}
	(*args)++;
	return (0);
}