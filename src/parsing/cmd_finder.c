/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_finder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 20:55:03 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/10 20:59:40 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

bool	is_point(t_cmd **cmd)
{
	return (!ft_strncmp((*cmd)->cmd, ".", 2)
		|| !ft_strncmp((*cmd)->cmd, "..", 3));
}

static int	is_full_path(t_cmd **cmd, int *exit_status)
{
	if (is_builtin((*cmd)->cmd))
	{
		(*cmd)->is_valid_cmd = true;
		return (1);
	}
	if (ft_strnstr((*cmd)->cmd, "/", ft_strlen((*cmd)->cmd)))
	{
		if (dir_error(cmd, exit_status))
			return (1);
		if (access((*cmd)->cmd, F_OK) == 0 && access((*cmd)->cmd, X_OK) == 0)
		{
			(*cmd)->is_valid_cmd = true;
			return (1);
		}
		else if (access((*cmd)->cmd, F_OK) == 0
			&& access((*cmd)->cmd, X_OK) != 0)
			return (error_permissions(cmd, exit_status));
		else
			return (error_command_not_found(cmd, exit_status));
	}
	return (0);
}

static int	cmd_no_path(t_cmd **cmd, char **path, int *exit_status)
{
	if (!path)
	{
		if (!is_full_path(cmd, exit_status))
		{
			return (error_command_not_found(cmd, exit_status));
		}
	}
	return (0);
}

static	int	cmd_for_path(t_cmd **cmd, char *path, int *exit_status)
{
	char	*tmp;
	char	*ex_path;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (-1);
	ex_path = ft_strjoin(tmp, (*cmd)->cmd);
	if (!ex_path)
	{
		free_null(tmp);
		return (-1);
	}
	free_null(tmp);
	if (access(ex_path, F_OK) == 0 && access(ex_path, X_OK) == 0)
	{
		free_null((*cmd)->cmd);
		(*cmd)->cmd = ex_path;
		(*cmd)->is_valid_cmd = true;
		return (1);
	}
	else if (access(ex_path, F_OK) == 0)
		return (free(ex_path), error_permissions(cmd, exit_status));
	free(ex_path);
	return (0);
}

int	get_cmd_path(t_cmd **cmd, char **path, int *exit_status)
{
	int		return_value;

	*exit_status = 0;
	if (cmd_no_path(cmd, path, exit_status) == 1)
		return (-1);
	if ((*cmd)->cmd && !is_full_path(cmd, exit_status) && !is_point(cmd))
	{
		while (path && *path)
		{
			return_value = cmd_for_path(cmd, *path, exit_status);
			if (return_value == 1)
				break ;
			else if (return_value == -1)
				return (-1);
			path++;
		}
	}
	return (cmd_not_found(cmd, exit_status));
}
