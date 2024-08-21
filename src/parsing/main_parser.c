/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 05:31:43 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/16 11:47:46 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	save_hdoc(t_cmd *cmd)
{
	size_t	i;
	size_t	j;
	size_t	k;

	while (cmd)
	{
		cmd->hdoc = ft_calloc(sizeof(char **), (ft_tablen(cmd->infile) + 1));
		i = 0;
		j = 0;
		k = 0;
		while (cmd->type_chain && cmd->type_chain[i]
			&& cmd->type_chain[i] != UNDEFINED)
		{
			if (cmd->type_chain[i] == HDOC || cmd->type_chain[i] == RED_IN)
			{
				if (cmd->type_chain[i] == HDOC)
					cmd->hdoc[j++] = &(cmd->infile[k]);
				k++;
			}
			i++;
		}
		cmd = cmd->next;
	}
	return (true);
}

int	parse(t_minishell *mshell)
{
	t_lexer	*lex;

	lex = lexer(mshell->line);
	if (expand(&lex, mshell->env, mshell->last_exit_status) == -1)
		return (free_lexer(lex), -1);
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
	save_hdoc(mshell->cmds);
	free_lexer(lex);
	return (0);
}
