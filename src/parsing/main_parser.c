/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 05:31:43 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/04 00:23:05 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse(t_minishell *mshell)
{
	t_lexer	*lex;

	lex = lexer(mshell->line);
	mshell->lex = lex;
	if (expand(&lex, mshell->env, mshell->last_exit_status) == -1)
		return (free_lexer(lex), -1);
	if (DEBUG)
		print_lexer(lex);
	if (!validate(lex, &(mshell->last_exit_status)))
	{
		free_lexer(lex);
		return (1);
	}
	mshell->cmds = lexer_to_cmd(lex, mshell->path, &(mshell->last_exit_status));
	if (!mshell->cmds)
	{
		free_cmds(mshell->cmds);
		return (1);
	}
	if (DEBUG)
		print_cmds(mshell->cmds);
	// get cmd fo all path
	free_lexer(lex);
	return (0);
}