/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:43:36 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/16 11:12:18 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Execute a command
 * @param mshell
 * @param cmd
 */
int	exec_cmd(t_minishell *mshell, t_cmd *cmd)
{
	if (is_builtin(cmd->cmd) == true)
	{
		if (exec_builtin(mshell, cmd))
			return (1);
	}
	else if (cmd->cmd && cmd->is_valid_cmd == true)
	{
		close(mshell->original_stdout);
		close(mshell->original_stdin);
		mshell->last_exit_status = 0;
		if (execve(cmd->cmd, cmd->args, mshell->env))
			return (perror("execve"), 127);
	}
	if (dup2(mshell->original_stdout, STDOUT_FILENO) == -1)
	{
		perror("dup2 failed");
		exit(free_shell(mshell, errno));
	}
	close(mshell->original_stdout);
	if (dup2(mshell->original_stdin, STDIN_FILENO) == -1)
	{
		perror("dup2 failed");
		exit(free_shell(mshell, errno));
	}
	return (close(mshell->original_stdin), 0);
}

int	dup_cmd(int i, t_cmd *cmd, int old[2], int new[2])
{
	if (i != 0)
	{
		if (dup2(old[0], STDIN_FILENO) == -1)
		{
			perror("dup2 failedregbverge");
			return (1);
		}
		if (old[0] != -1)
			close(old[0]);
		if (old[1] != -1)
			close(old[1]);
	}
	if (cmd->next)
	{
		if (dup2(new[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 failed");
			return (1);
		}
		if (new[0] != -1)
			close(new[0]);
		if (new[1] != -1)
			close(new[1]);
	}
	return (0);
}

int	process_commands(t_minishell *mshell, int old[2], int new[2])
{
	t_cmd	*cmd;
	int		i;

	if (set_default_fd(mshell, old, new) == 1)
		return (1);
	i = 0;
	cmd = mshell->cmds;
	while (cmd)
	{
		handle_signal_process();
		if (pipe_command(cmd, new) == 1)
			return (1);
		if (fork_exec(mshell, old, new, i))
			return (1);
		close_and_cpy(old, new, i);
		next_command(&cmd, &i);
	}
	reset_fds(mshell, old, new);
	return (0);
}

void	return_status(t_minishell *mshell, int status)
{
	int	i;
	int	size;

	i = -1;
	size = lst_size(mshell->cmds);
	while (++i < size)
	{
		if (waitpid(-1, &status, 0) == mshell->last_pid)
		{
			if ((((status) & 0x7f) == 0))
				mshell->last_exit_status = (((status) & 0xff00) >> 8);
			else if ((((signed char)(((status) & 0x7f) + 1) >> 1) > 0))
				mshell->last_exit_status = ((status) & 0x7f) + 128;
		}
	}
}

int	exec(t_minishell *mshell)
{
	int	old[2];
	int	new[2];
	int	status;

	status = 0;
	if (init_exec(old, new, mshell) == 1)
		return (reset_fds(mshell, old, new), 1);
	if (process_commands(mshell, old, new) == 1)
		return (reset_fds(mshell, old, new), 1);
	return_status(mshell, status);
	if (g_sig == SIGINT)
	{
		mshell->last_exit_status = 130;
		rl_done = 1;
	}
	return (ft_close(old, new), 0);
}
