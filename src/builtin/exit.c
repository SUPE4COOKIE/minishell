/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:04:04 by scrumier          #+#    #+#             */
/*   Updated: 2024/05/27 10:54:46 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** @brief: Free all allocated memory and exit the shell
** @param mshell The minishell structure
** @param status The exit status
*/
int	free_shell(t_minishell *mshell, int status)
{
	int	i;

	if (mshell->line)
		free(mshell->line);
	if (mshell->current_path)
		free(mshell->current_path);
	if (mshell->pwd)
		free(mshell->pwd);
	if (mshell->old_pwd)
		free(mshell->old_pwd);
	if (mshell->path)
		free(mshell->path);
	if (mshell->env)
	{
		i = 0;
		while (mshell->env[i])
			free(mshell->env[i++]);
		free(mshell->env);
	}
	return(status);
}

/*
** @brief: Check if the command is between two commands
** @param mshell The minishell structure
** @return true if the command is between two commands, false otherwise
*/
static bool	get_is_between_cmd(t_minishell *mshell)
{
	t_cmd	*cmd;

	cmd = mshell->cmd;
	if (cmd)
		if (cmd->next && cmd->prev)
			return (true);
	return (false);
}

/*
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
	is_between = get_is_between_cmd(args[1]);
	if (is_between)
		ft_putstr_fd("exit\n", 2);
	if (args[2])
		return (error_cmd(mshell, 1, "exit: too many arguments"));
	if (args[1])
	{
		status = ft_atol(args[1]);
		if (status > INT_MAX) // Check long max
			return (error_cmd(mshell, 1, "exit: numeric argument required"));
	}
	return (free_shell(mshell, status));
}
