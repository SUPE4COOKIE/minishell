/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:32:46 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/16 13:02:49 by scrumier         ###   ########.fr       */
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
	if (g_sig == SIGINT)
		ft_putstr_fd("\n", 2);
	else if (g_sig == SIGQUIT)
		ft_putstr_fd("Quit: 3\n", 2);
}

int	event(void)
{
	return (0);
}
