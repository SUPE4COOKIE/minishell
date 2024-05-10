/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 02:06:43 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/05/10 02:17:36 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	things to check for each:

	- pipe:
		- no pipe at the end
		- no pipe at the start
		- should be between 2 words
	
	- redirections:
		- followed by a word
		- no consecutive operations without a word in between
	
	- here_doc:
		- followed by a word
		- no consecutive operations without a word in between
*/

#include "minishell.h"

int	validate(t_lexer *lex)
{
	t_lexer	*tmp;

	tmp = lex;
	while (tmp)
	{
		if (tmp->type == T_PIPE)
		{
			if (!tmp->next || !tmp->prev || tmp->prev->type != T_WORD || tmp->next->type != T_WORD)
				return (0);
		}
		else if (tmp->type == T_REDIR_IN || tmp->type == T_REDIR_OUT || tmp->type == T_APPEND_OUT)
		{
			if (!tmp->next || tmp->next->type != T_WORD)
				return (0);
			if (tmp->prev && (tmp->prev->type == T_REDIR_IN || tmp->prev->type == T_REDIR_OUT || tmp->prev->type == T_APPEND_OUT))
				return (0);
		}
		else if (tmp->type == T_HERE_DOC)
		{
			if (!tmp->next || tmp->next->type != T_WORD)
				return (0);
			if (tmp->prev && (tmp->prev->type == T_REDIR_IN || tmp->prev->type == T_REDIR_OUT || tmp->prev->type == T_APPEND_OUT))
				return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}