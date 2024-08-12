/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 11:29:55 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/12 11:30:13 by scrumier         ###   ########.fr       */
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
