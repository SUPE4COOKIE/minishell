/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:57:14 by scrumier          #+#    #+#             */
/*   Updated: 2024/06/14 19:18:45 by mwojtasi         ###   ########.fr       */
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
void	exec_builtin(t_minishell *mshell, t_cmd *cmd)
{
	if (ft_strncmp(cmd->cmd, "echo", 5) == 0)
		builtin_echo(mshell, cmd->args);
	if (ft_strncmp(cmd->cmd, "cd", 3) == 0)
		builtin_cd(mshell, cmd->args);
	if (ft_strncmp(cmd->cmd, "pwd", 4) == 0)
		builtin_pwd(mshell);
	if (ft_strncmp(cmd->cmd, "export", 7) == 0)
		builtin_export(mshell, cmd->args);
	if (ft_strncmp(cmd->cmd, "unset", 6) == 0)
		builtin_unset(mshell, cmd->args);
	if (ft_strncmp(cmd->cmd, "env", 4) == 0)
		builtin_env(mshell, cmd->args);
	if (ft_strncmp(cmd->cmd, "exit", 6) == 0)
		builtin_exit(mshell, cmd->args);
}
