/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_finder_errors.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 18:09:49 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/10 21:15:49 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

int	dir_error(t_cmd **cmd, int *exit_status)
{
	char		*tmp;
	struct stat	buf;

	if (stat((*cmd)->cmd, &buf) == -1)
	{
		(*cmd)->is_valid_cmd = false;
		perror((*cmd)->cmd);
		return (-1);
	}
	if (((buf.st_mode) & 0170000) == (0040000))
	{
		(*cmd)->is_valid_cmd = false;
		*exit_status = 126;
		tmp = ft_strjoin((*cmd)->cmd, ": is a directory\n");
		if (!tmp)
			return (-1);
		write(2, tmp, ft_strlen(tmp));
		free(tmp);
		return (1);
	}
	return (0);
}

int	error_permissions(t_cmd **cmd, int *exit_status)
{
	char	*tmp;

	*exit_status = 126;
	tmp = ft_strjoin((*cmd)->cmd, ": permission denied\n");
	if (!tmp)
		return (-1);
	write(2, tmp, ft_strlen(tmp));
	(*cmd)->is_valid_cmd = false;
	free((*cmd)->cmd);
	free(tmp);
	(*cmd)->cmd = NULL;
	return (2);
}

int	error_command_not_found(t_cmd **cmd, int *exit_status)
{
	char	*tmp;

	*exit_status = 127;
	tmp = ft_strjoin((*cmd)->cmd, ": command not found\n");
	if (!tmp)
		return (-1);
	write(2, tmp, ft_strlen(tmp));
	(*cmd)->is_valid_cmd = false;
	free(tmp);
	free((*cmd)->cmd);
	(*cmd)->cmd = NULL;
	return (2);
}

int	cmd_not_found(t_cmd **cmd, int *exit_status)
{
	if (!(*cmd)->cmd || is_builtin((*cmd)->cmd))
		return (0);
	if (!ft_strnstr((*cmd)->cmd, "/", ft_strlen((*cmd)->cmd)) || is_point(cmd))
		return (error_command_not_found(cmd, exit_status));
	return (0);
}
