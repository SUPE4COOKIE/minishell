/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mshell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:26:50 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/14 11:26:54 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_minishell *mshell)
{
	mshell->env = NULL;
	mshell->path = NULL;
	mshell->cmds = NULL;
	mshell->last_exit_status = 0;
	mshell->in_heredoc = false;
	mshell->last_pid = 0;
	mshell->original_stdout = 0;
	mshell->original_stdin = 0;
	mshell->invalid_redir = NULL;
	mshell->cmds = NULL;
	g_sig = 0;
	rl_done = 0;
}
