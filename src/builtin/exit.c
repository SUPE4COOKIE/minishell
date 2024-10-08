/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:04:04 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/16 13:22:53 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (cmd->next)
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
	bool	is_overflow;

	is_overflow = false;
	if (get_is_between_cmd(mshell) == false)
		ft_putstr_fd("exit\n", 2);
	if (args)
	{
		if (args[1] && isnumber(args[1]) == false)
			exit(free_shell(mshell, 2));
		else if (args[1] && args[2] && mshell->cmds->next)
			exit(error_cmd(mshell, 1, "exit: too many arguments"));
		else if (args[1] && args[2])
			return (error_cmd(mshell, 1, "exit: too many arguments"));
		else if (args[1])
		{
			mshell->last_exit_status = atoutint8(args[1], &is_overflow);
			close(mshell->original_stdin);
			close(mshell->original_stdout);
			if (is_overflow == true)
				exit(error_cmd(mshell, 2, "exit: numeric argument \
					required"));
		}
		exit(free_shell(mshell, mshell->last_exit_status));
	}
	return (0);
}
