/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 11:29:55 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/16 11:36:00 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	next_command(t_cmd **cmd, int *i)
{
	*cmd = (*cmd)->next;
	(*i)++;
}

void	handle_signal_process(void)
{
	signal(SIGINT, signal_exec);
	signal(SIGQUIT, signal_exec);
}

int	pipe_command(t_cmd *cmd, int new[2])
{
	if (cmd->next)
	{
		if (pipe(new) == -1)
		{
			perror("pipe failed");
			return (1);
		}
	}
	return (0);
}

void	init_fds(t_fds *fds, int old[2], int new[2])
{
	fds->old[0] = old[0];
	fds->old[1] = old[1];
	fds->new[0] = new[0];
	fds->new[1] = new[1];
}
