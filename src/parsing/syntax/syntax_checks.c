/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 21:26:52 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/06/25 15:28:41 by mwojtasi         ###   ########.fr       */
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
			if (!(tmp->next->type == T_WORD || tmp->next->type == T_D_QUOTED_WORD || tmp->next->type == T_S_QUOTED_WORD))
				return (syntax_error(tmp->next->value, exit_code));
		}
		tmp = tmp->next;
	}
	return (true);
}

bool	is_valid_quotes(char *line, int *status_code)
{
	size_t	s_quotes;
	size_t	d_quotes;
	char	in_quote;
	size_t	i;

	s_quotes = 0;
	d_quotes = 0;
	i = 0;
	in_quote = 0;
	while (line[i])
	{
		is_in_quote(line[i], &in_quote);
		if (line[i] == '\'' && in_quote != '"')
			s_quotes++;
		else if (line[i] == '"' && in_quote != '\'')
			d_quotes++;
		i++;
	}
	if (s_quotes % 2)
		return (syntax_error("'", status_code));
	if (d_quotes % 2)
		return (syntax_error("\"", status_code));
	return (true);
}
