/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 13:05:02 by sonamcrumie       #+#    #+#             */
/*   Updated: 2024/05/21 15:21:44 by scrumier         ###   ########.fr       */
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

	init_old_new(old, new);
	cmd = mshell->cmd;
	while (cmd)
	{
		if (cmd->next && pipe(new) == -1) // sauf si derniere commande
			error_pipe("pipe failed", new, old, cmd);
		id = fork();
		if (id == -1)
			error_pipe("fork failed", new, old, cmd);
		if (id == 0)
		{
			if (dup2(old[0], STDIN_FILENO))
				error_pipe("dup2 failed", new, old, cmd);
			if (dup2(new[1], STDOUT_FILENO))
				error_pipe("dup2 failed", new, old, cmd);
			ft_close(old, new);
			execve(cmd->cmd[0], cmd->cmd, mshell->env);
			free_shell(mshell, 1);
			perror("execve"); // libere la memoire

		}
		if (old[0])
			close(old[0]);
		if (old[1])
			close(old[1]);
		old[0] = new[0];
		old[1] = new[1];
	}
	while (waitpid(id, &mshell->last_exit_status, 0) > 0)
		;
}
