/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:57:14 by scrumier          #+#    #+#             */
/*   Updated: 2024/06/25 11:44:22 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if the command is a builtin command
 * @param cmd
 * @return true if the command is a builtin command, false otherwise
 */
bool	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (true);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (true);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (true);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (true);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (true);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (true);
	if (ft_strncmp(cmd, "exit", 6) == 0)
		return (true);
	return (false);
}

/**
 * @brief Execute a builtin command
 * @param mshell
 * @param cmd
 */
int	exec_builtin(t_minishell *mshell, t_cmd *cmd)
{
	if (ft_strncmp(cmd->cmd, "echo", 5) == 0)
		if (builtin_echo(mshell, cmd->args))
			return (error_msg("echo failed"));
	if (ft_strncmp(cmd->cmd, "cd", 3) == 0)
		if (builtin_cd(mshell, cmd->args))
			return (error_msg("cd failed"));
	if (ft_strncmp(cmd->cmd, "pwd", 4) == 0)
		if (builtin_pwd(mshell))
			return (error_msg("pwd failed"));
	if (ft_strncmp(cmd->cmd, "export", 7) == 0)
		if (builtin_export(mshell, cmd->args))
			return (error_msg("export failed"));
	if (ft_strncmp(cmd->cmd, "unset", 6) == 0)
		if (builtin_unset(mshell, cmd->args))
			return (error_msg("unset failed"));
	if (ft_strncmp(cmd->cmd, "env", 4) == 0)
		if (builtin_env(mshell, cmd->args))
			return (error_msg("env failed"));
	if (ft_strncmp(cmd->cmd, "exit", 6) == 0)
		if (builtin_exit(mshell, cmd->args))
			return (error_msg("exit failed"));
	return (0);
}
