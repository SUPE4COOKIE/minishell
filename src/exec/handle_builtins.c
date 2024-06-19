/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:57:14 by scrumier          #+#    #+#             */
/*   Updated: 2024/06/12 14:58:31 by scrumier         ###   ########.fr       */
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
	if (ft_strncmp(cmd, "echo", ft_strlen("echo") + 1) == 0)
		return (true);
	if (ft_strncmp(cmd, "cd", ft_strlen("cd") + 1) == 0)
		return (true);
	if (ft_strncmp(cmd, "pwd", ft_strlen("pwd") + 1) == 0)
		return (true);
	if (ft_strncmp(cmd, "export", ft_strlen("export")) + 1 == 0)
		return (true);
	if (ft_strncmp(cmd, "unset", ft_strlen("unset") + 1) == 0)
		return (true);
	if (ft_strncmp(cmd, "env", ft_strlen("env") + 1) == 0)
		return (true);
	if (ft_strncmp(cmd, "exit", ft_strlen("exit") + 1) == 0)
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
	if (ft_strncmp(cmd->cmd, "echo", ft_strlen(cmd->cmd)) == 0)
		builtin_echo(mshell, cmd->args);
	if (ft_strncmp(cmd->cmd, "cd", ft_strlen(cmd->cmd)) == 0)
		builtin_cd(mshell, cmd->args);
	if (ft_strncmp(cmd->cmd, "pwd", ft_strlen(cmd->cmd)) == 0)
		builtin_pwd(mshell);
	if (ft_strncmp(cmd->cmd, "export", ft_strlen(cmd->cmd)) == 0)
		builtin_export(mshell, cmd->args);
	if (ft_strncmp(cmd->cmd, "unset", ft_strlen(cmd->cmd)) == 0)
		builtin_unset(mshell, cmd->args);
	if (ft_strncmp(cmd->cmd, "env", ft_strlen(cmd->cmd)) == 0)
		builtin_env(mshell, cmd->args);
	if (ft_strncmp(cmd->cmd, "exit", ft_strlen(cmd->cmd)) == 0)
		builtin_exit(mshell, cmd->args);
}
