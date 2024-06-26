/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:43:36 by scrumier          #+#    #+#             */
/*   Updated: 2024/06/14 14:30:33 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Copy the arguments
 * @param args
 * @return The copied arguments
 */
char	**copy_args(char **args)
{
	int		arg_count;
	char	**args_copy;
	int		i;

	arg_count = 0;
	args_copy = NULL;
	i = 0;
	while (args[arg_count])
		arg_count++;
	args_copy = malloc((arg_count + 1) * sizeof(char *));
	if (args_copy == NULL)
		return (NULL);
	while (args[i] != NULL)
	{
		args_copy[i] = ft_strdup(args[i]);
		if (args_copy[i] == NULL)
			return (free_tab(args_copy), NULL);
		i++;
	}
	args_copy[arg_count] = NULL;
	return (args_copy);
}

/**
 * @brief Execute a command
 * @param mshell
 * @param cmd
 */
void	exec_cmd(t_minishell *mshell, t_cmd *cmd)
{
	struct sigaction sa;

	if (is_builtin(cmd->cmd) == true)
		exec_builtin(mshell, cmd);
	else
	{
		sa.sa_handler = SIG_DFL;
		sa.sa_flags = SA_SIGINFO;
		sigemptyset(&sa.sa_mask);
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
		execve(cmd->cmd, cmd->args, mshell->env);
	}
}

/**
 * @brief Duplicate the file descriptors
 * @param i
 * @param cmd
 * @param old
 * @param new
 */
void	dup_cmd(int i, t_cmd *cmd, int old[2], int new[2])
{
	if (i != 0)
	{
		if (dup2(old[0], STDIN_FILENO) == -1)
		{
			perror("dup2 failed");
			exit(EXIT_FAILURE);
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
			perror("dup2(2) failed");
			exit(EXIT_FAILURE);
		}
		if (new[0] != -1)
			close(new[0]);
		if (new[1] != -1)
			close(new[1]);
	}
}

/**
 * @brief Execute de command ?, pipe
 * @param mshell
 * @param old
 * @param new
 */
void	process_commands(t_minishell *mshell, int old[2], int new[2])
{
	t_cmd	*cmd;
	int		i;

	i = 0;
	cmd = mshell->cmds;
	signal(SIGINT, signal_exec);
	signal(SIGQUIT, signal_exec);
	while (cmd)
	{
		if (cmd->is_valid_cmd == false)
		{
			cmd = cmd->next;
			continue ;
		}
		if (cmd->next)
			if (pipe(new) == -1)
				error_pipe("pipe failed", new, old, cmd);
		fork_exec(mshell, old, new, i);
		i++;
		cmd = cmd->next;
	}
}

int	lst_size(t_cmd *cmd)
{
	int i;

	i = 0;
	while (cmd)
	{
		i++;
		cmd = cmd->next;
	}
	return (i);
}

/**
 * @brief Execute the minishell
 * @param mshell
 */
void	exec(t_minishell *mshell)
{
	int	old[2];
	int	new[2];
	int status;
	int i;
	int size;

	i = 0;
	size = lst_size(mshell->cmds);
	status = 0;
	init_exec(old, new, mshell);
	process_commands(mshell, old, new);
	ft_close(old, new);
	while (i < size)
	{
		if (waitpid(-1 , &status, 0) == mshell->last_pid)
		{
			if (WIFEXITED(status))
				mshell->last_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				mshell->last_exit_status = WTERMSIG(status) + 128;
		}
		i++;
	}
	if (g_sig == SIGINT)
		printf("\n");
	else if (g_sig == SIGQUIT)
		printf("Quit (core dumped)\n");
}
