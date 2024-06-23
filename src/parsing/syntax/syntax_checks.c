/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 21:26:52 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/06/23 08:17:48 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	things to check for each:

	- pipe:
		- no pipe at the end
		- no pipe at the start
		- should be between 2 words // can have an operation after
	
	- redirections:
		- followed by a word
		- no consecutive operations without a word in between
	
	- here_doc:
		- followed by a word
		- no consecutive operations without a word in between
*/

#include "minishell.h"

static inline bool syntax_error(char *message, int *status_code)
{
	printf("minishell: syntax error near unexpected token `%s'\n", message);
	*status_code = 2;
	return (false);
}
bool	validate(t_lexer *lex, int *exit_code)
{
	t_lexer	*tmp;

	tmp = lex;
	while (tmp)
	{
		if (tmp->type == T_PIPE)
		{
			if (!tmp->next || !tmp->prev || tmp->next->type == T_PIPE)
				return (syntax_error("|", exit_code));
		}
		else if (tmp->type == T_REDIR_IN || tmp->type == T_REDIR_OUT || tmp->type == T_APPEND_OUT || tmp->type == T_HERE_DOC)
		{
			if (!tmp->next)
				return (syntax_error("newline", exit_code));
			if (tmp->next->type != T_WORD || tmp->next->type != T_D_QUOTED_WORD || tmp->next->type != T_S_QUOTED_WORD || tmp->next->type != T_HERE_DOC)
				return (syntax_error(tmp->next->value, exit_code));
		}
		tmp = tmp->next;
	}
	return (1);
}