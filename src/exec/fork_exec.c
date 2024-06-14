/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:19:44 by scrumier          #+#    #+#             */
/*   Updated: 2024/06/14 14:35:10 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	cmd = mshell->cmds;
	if (is_builtin(cmd->cmd) == false)
		id = fork();
	else if (is_builtin(cmd->cmd) == true && cmd->next)
		id = fork();
	else
		id = 0;
	if (id == -1)
		error_pipe("fork failed", new, old, cmd);
	if (id == 0)
	{
		dup_cmd(i, cmd, old, new);
		handle_file_redirection(cmd, old, new);
		exec_cmd(mshell, cmd);
		if (is_builtin(cmd->cmd) == false)
			exit(EXIT_FAILURE);
		return ;
	}
	else
	{
		close_old(i, old);
		old[0] = new[0];
		old[1] = new[1];
	}

}
