/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 13:54:19 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/12 14:32:25 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_new_line(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_sig = SIGINT;
	}
	else if (sig == SIGQUIT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_sig = 0;
	}
}

void	signal_here_doc(int signal)
{
	g_sig = signal;
	if (g_sig == SIGINT)
	{
		rl_done = 1;
	}
}

void	signal_exec(int signal)
{
	g_sig = signal;
	(void)signal;
}

int	event(void)
{
	return (0);
}

void	init(t_minishell *mshell)
{
	mshell->env = NULL;
	mshell->path = NULL;
	mshell->line = NULL;
	mshell->last_exit_status = 0;
	mshell->in_heredoc = false;
	mshell->last_pid = 0;
	mshell->original_stdin = 0;
	mshell->original_stdout = 0;
	mshell->invalid_redir = NULL;
	mshell->cmds = NULL;
	g_sig = 0;
	rl_done = 0;
}
