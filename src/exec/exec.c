/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 13:05:02 by sonamcrumie       #+#    #+#             */
/*   Updated: 2024/05/28 16:49:55 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_close(int old[2], int new[2])
{
	if (old[0])
		close(old[0]);
	if (old[1])
		close(old[1]);
	if (new[0])
		close(new[0]);
	if (new[1])
		close(new[1]);
}

static void	init_old_new(int old[2], int new[2])
{
	old[0] = -1;
	old[1] = -1;
	new[0] = -1;
	new[1] = -1;
}



void	exec(t_minishell *mshell)
{
	t_cmd	*cmd;
	int		id;
	int		old[2];
	int		new[2];
	int		fd[2];

	init_old_new(old, new);
	cmd = mshell->cmds;
	while (cmd)
	{
		if (cmd->next)
			if (pipe(new) == -1) // sauf si derniere commande
				error_pipe("pipe failed", new, old, cmd);
		id = fork();
		if (id == -1)
			error_pipe("fork failed", new, old, cmd);
		if (id == 0)
		{
			if (old[0] != -1 && old[1] != -1)
				fd[0] = old[0];
			else
				fd[0] = 0;
			if (cmd->next)
				fd[1] = new[1];
			else
				fd[1] = 1;
			if (dup2(fd[0], STDIN_FILENO) == -1)
				error_pipe("dup2 failedd", new, old, cmd);
			if (dup2(fd[1], STDOUT_FILENO) == -1)
				error_pipe("dup2 failed", new, old, cmd);
			ft_close(old, new);
			execve(cmd->cmd, cmd->args, mshell->env);
			//free_shell(mshell, 1);
			perror("execve");
		}
		old[0] = new[0];
		old[1] = new[1];
		cmd = cmd->next;
	}
	waitpid(id, &mshell->last_exit_status, 0);
}
