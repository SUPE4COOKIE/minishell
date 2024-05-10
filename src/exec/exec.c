/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sonamcrumiere <sonamcrumiere@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 13:05:02 by sonamcrumie       #+#    #+#             */
/*   Updated: 2024/05/10 14:42:26 by sonamcrumie      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void in_cmd(t_minishell *mshell, t_cmd *cmd)
{
	char *command = cmd->cmd[0];
	cmd->cmd++;
	char **args = cmd->cmd;
	if (dup2(mshell->cmd->pipe, STDOUT_FILENO) == -1)
		exit(1);
	close(mshell->cmd->pipe);
	execve(cmd->cmd[0], args, mshell->env);
	perror("execve");
	return ;
}

void exec(t_minishell *mshell)
{
	t_cmd *cmd = mshell->cmd;

	while (cmd->next)
	{
		if (pipe(mshell->cmd->pipe) == -1)
			exit(1);

		if (cmd)
		{
			cmd->pid = fork();
			if (cmd->pid == -1)
				exit(1);
			if (cmd->pid == 0)
				in_cmd(mshell, cmd);
		}
		if (cmd->next)
		{
			cmd->pid = fork();
			if (cmd->pid == -1)
				exit(1);
			if (cmd->pid == 0)
				out_cmd(mshell, cmd);
		}
		close(cmd->pipe);
		close(cmd->next->pipe);
	}
}
