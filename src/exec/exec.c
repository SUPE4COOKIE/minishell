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
void exec_cmd(t_minishell *mshell, t_cmd *cmd)
{
	char *program;
	char **args;

	if (is_builtin(cmd->cmd) == true)
		exec_builtin(mshell, cmd);
	else
	{
		program = ft_strdup(cmd->cmd);
		if (program == NULL) {
			perror("Failed to allocate memory for program path");
			return ;
		}
		args = copy_args(cmd->args);
		if (args == NULL) {
			perror("Failed to allocate memory for arguments");
			free(program);
			return ;
		}
		execve(program, args, mshell->env);
		free(program);
		free_tab(args);
	}
}

/**
 * @brief Duplicate the file descriptors
 * @param i
 * @param cmd
 * @param old
 * @param new
 */
void dup_cmd(int i, t_cmd *cmd, int old[2], int new[2])
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
			perror("dup2 failed");
			exit(EXIT_FAILURE);
		}
		if (new[0] != -1)
			close(new[0]);
		if (new[1] != -1)
			close(new[1]);
	}
}

/*
** @brief execute the commands
** @param mshell The minishell structure
*/
void	exec(t_minishell *mshell)
{
	t_cmd	*cmd;
	int		old[2];
	int		new[2];
	int		i;

	init_exec(old, new, mshell);
	cmd = mshell->cmds;
	i = 0;
	while (cmd)
	{
		if (cmd->is_valid_cmd == false)
		{
			cmd = cmd->next;
			continue ;
		}
		if (cmd->next)
		{
			if (pipe(new) == -1)
				error_pipe("pipe failed", new, old, cmd);
		}
		fork_exec(mshell, old, new, i);
		i++;
		cmd = cmd->next;
	}
	ft_close(old, new);
	while (waitpid(-1, NULL, 0) != -1)
		;
}
