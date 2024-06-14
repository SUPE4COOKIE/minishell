/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:04:04 by scrumier          #+#    #+#             */
/*   Updated: 2024/05/30 16:37:05 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void del_cmd(t_cmd *cmd)
{
	if (cmd->args)
		free_tab(cmd->args);
	cmd->args = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	free(cmd);
	cmd = NULL;
}

void free_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd;
		cmd = cmd->next;
		del_cmd(tmp);
	}
}

void free_mshell(t_minishell *mshell)
{
	if (mshell->env)
		free_tab(mshell->env);
	mshell->env = NULL;
	if (mshell->path)
		free_tab(mshell->path);
	mshell->path = NULL;
	if (mshell->line)
		free(mshell->line);
	mshell->line = NULL;
	if (mshell->cmds)
		free_cmds(mshell->cmds);
	mshell->cmds = NULL;
	mshell = NULL;
}

/**
** @brief: Free all allocated memory and exit the shell
** @param mshell The minishell structure
** @param status The exit status
*/
int	free_shell(t_minishell *mshell, int status)
{
	free_mshell(mshell);
	return (status);
}

/**
** @brief: Check if the command is between two commands
** @param mshell The minishell structure
** @return true if the command is between two commands, false otherwise
*/
static bool	get_is_between_cmd(t_minishell *mshell)
{
	t_cmd	*cmd;

	cmd = mshell->cmds;
	if (cmd)
		if (cmd->next && cmd->prev)
			return (true);
	return (false);
}

/**
** @brief: Exit the shell
** @param mshell The minishell structure
** @param args The arguments
** @return Exit status
*/
int	builtin_exit(t_minishell *mshell, char **args)
{
	uint8_t status;
	bool	is_between;

	status = mshell->last_exit_status;
	is_between = get_is_between_cmd(mshell);
	if (is_between)
		ft_putstr_fd("exit\n", 2);
	if (args[2])
		return (error_cmd(mshell, 1, "exit: too many arguments"));
	if (args[1])
		status = ft_atoi(args[1]);
	exit(free_shell(mshell, status));
}
