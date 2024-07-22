/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:19:44 by scrumier          #+#    #+#             */
/*   Updated: 2024/07/22 14:18:18 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Close the old file descriptors and cpy new to old
 * @param i The index
 * @param old The old file descriptors
 */
void	close_and_cpy(int old[2], int new[2], int i)
{
	(void)i;
	close_old(i, old);
	old[0] = new[0];
	old[1] = new[1];
}

/**
 * @brief Fork and execute a command
 * @param mshell
 * @param old
 * @param new
 * @param i
 */
void	fork_exec(t_minishell *mshell, int old[2], int new[2], int i)
{
	pid_t	id;
	t_cmd	*cmd;
	int		y;

	y = 0;
	cmd = mshell->cmds;
	id = 0;
	while (y++ < i)
		cmd = cmd->next;
	if (is_builtin(cmd->cmd) == false || \
			(is_builtin(cmd->cmd) == true && cmd->next))
			id = fork();
	if (id != 0)
		mshell->last_pid = id;
	if (id == -1)
		error_pipe("fork failed", new, old, cmd);
	if (id == 0)
	{
		dup_cmd(i, cmd, old, new);
		handle_file_redirection(mshell, cmd, old, new);
		if (cmd->is_valid_cmd == false)
			return ;
		exec_cmd(mshell, cmd);
		if (is_builtin(cmd->cmd) == false || \
				(is_builtin(cmd->cmd) == true && cmd->next))
			exit(EXIT_FAILURE);
	}
	close_and_cpy(old, new, i);
}
