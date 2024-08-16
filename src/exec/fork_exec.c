/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:19:44 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/16 12:58:21 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	handle_error_exec(t_minishell *mshell, t_cmd *cmd, \
		int old[2], int new[2])
{
	if (handle_file_redirection(mshell, cmd, old, new) == 1)
	{
		ft_close(old, new);
		close(mshell->original_stdout);
		close(mshell->original_stdin);
		free_mshell(mshell);
		exit(1);
	}
	if (cmd->is_valid_cmd == false)
	{
		free_mshell(mshell);
		close(mshell->original_stdout);
		close(mshell->original_stdin);
		exit(127);
	}
}

int	manage_cmd(t_minishell *mshell, t_cmd *cmd, int i, t_fds fds)
{
	int	old[2];
	int	new[2];

	old[0] = fds.old[0];
	old[1] = fds.old[1];
	new[0] = fds.new[0];
	new[1] = fds.new[1];
	handle_error_exec(mshell, cmd, old, new);
	dup_cmd(i, cmd, old, new);
	ft_close(old, new);
	if (exec_cmd(mshell, cmd))
		return (1);
	if (is_builtin(cmd->cmd) == false || \
			(is_builtin(cmd->cmd) == true && mshell->cmds->next))
	{
		close_free(mshell, old, new);
		exit(0);
	}
	return (0);
}

int	fork_exec(t_minishell *mshell, int old[2], int new[2], int i)
{
	pid_t	id;
	t_cmd	*cmd;
	int		y;
	t_fds	fds;

	init_fds(&fds, old, new);
	cmd = init_before_fork(&y, mshell, &id, i);
	if (is_builtin(cmd->cmd) == false || \
			(is_builtin(cmd->cmd) == true && mshell->cmds->next))
		id = fork();
	if (id == -1)
		return (error_msg("fork failed"));
	else if (id != 0)
	{
		mshell->last_pid = id;
		signal(SIGINT, SIG_IGN);
	}
	else if (id == 0)
		if (process_child(mshell, cmd, i, fds))
			return (1);
	return (0);
}

int	process_child(t_minishell *mshell, t_cmd *cmd, int i, t_fds fds)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (manage_cmd(mshell, cmd, i, fds))
		return (1);
	if (is_builtin(cmd->cmd) == false || \
			(is_builtin(cmd->cmd) == true && mshell->cmds->next))
	{
		exit(0);
	}
	return (0);
}
