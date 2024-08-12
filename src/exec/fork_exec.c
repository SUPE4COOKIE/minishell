/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:19:44 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/12 14:39:09 by scrumier         ###   ########.fr       */
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

t_cmd	*init_before_fork(int *y, t_minishell *mshell, pid_t *id, int i)
{
	t_cmd	*cmd;

	*y = 0;
	cmd = mshell->cmds;
	*id = 0;
	while ((*y)++ < i)
		cmd = cmd->next;
	return (cmd);
}

int	manage_pipes(t_minishell *mshell, t_cmd *cmd, int i, t_fds fds)
{
	int	old[2];
	int	new[2];

	old[0] = fds.old[0];
	old[1] = fds.old[1];
	dup_cmd(i, cmd, old, new);
	handle_file_redirection(mshell, cmd, old, new);
	if (cmd->is_valid_cmd == false)
		return (0);
	ft_close(old, new);
	if (exec_cmd(mshell, cmd))
		return (1);
	if (is_builtin(cmd->cmd) == false || \
			(is_builtin(cmd->cmd) == true && cmd->next))
	{
		free_env_path(mshell);
		free_cmds(mshell->cmds);
		exit(0);
	}
	return (0);
}

/**
 * @brief Fork and execute a command
 * @param mshell
 * @param old
 * @param new
 * @param i
 */
int	fork_exec(t_minishell *mshell, int old[2], int new[2], int i)
{
	pid_t	id;
	t_cmd	*cmd;
	int		y;
	t_fds	fds;

	fds.old[0] = old[0];
	fds.old[1] = old[1];
	fds.new[0] = new[0];
	fds.new[1] = new[1];
	cmd = init_before_fork(&y, mshell, &id, i);
	if (is_builtin(cmd->cmd) == false || \
			(is_builtin(cmd->cmd) == true && cmd->next))
		id = fork();
	if (id == -1)
		return (error_msg("fork failed"));
	else if (id != 0)
		mshell->last_pid = id;
	else if (id == 0)
	{
		if (manage_pipes(mshell, cmd, i, fds))
			return (1);
	}
	close_and_cpy(old, new, i);
	return (0);
}
