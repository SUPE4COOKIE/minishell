/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 05:31:43 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/06/20 21:51:11 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse(t_minishell *mshell)
{
	t_lexer	*lex;

	lex = lexer(mshell->line);
	save_path(mshell, mshell->env);
	expand(&lex, mshell->env, mshell->last_exit_status);
	if (DEBUG)
		print_lexer(lex);
	mshell->cmds = lexer_to_cmd(lex, mshell->path, &(mshell->last_exit_status));
	if (DEBUG)
		print_cmds(mshell->cmds);
	// get cmd fo all path
	free_lexer(lex);
	return (0);
}