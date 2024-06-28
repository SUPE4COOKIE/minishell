/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_finder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 20:55:03 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/06/28 14:42:54 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

int	dir_error(t_cmd **cmd, int *exit_status)
{
	char	*tmp;
	struct	stat buf;

	if (stat((*cmd)->cmd, &buf) == -1)
	{
		(*cmd)->is_valid_cmd = false;
		perror((*cmd)->cmd);
		return (1);
	}
	if (((buf.st_mode) & 0170000) == (0040000))
	{
		(*cmd)->is_valid_cmd = false;
		*exit_status = 126;
		tmp = ft_strjoin((*cmd)->cmd, ": is a directory\n");
		if (!tmp)
			exit(1); // TODO: add a proper exit struct
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
		exit(1); // TODO: add a proper exit struct
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
		exit(1); // TODO: add a proper exit struct
	write(2, tmp, ft_strlen(tmp));
	(*cmd)->is_valid_cmd = false;
	free(tmp);
	free((*cmd)->cmd);
	(*cmd)->cmd = NULL;
	return (2);
}

static int is_full_path(t_cmd **cmd, int *exit_status)
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
		else if (access((*cmd)->cmd, F_OK) == 0 && access((*cmd)->cmd, X_OK) != 0)
			return (error_permissions(cmd, exit_status));
		else
			return (error_command_not_found(cmd, exit_status));
	}
	return (0);
}

static void cmd_no_path(t_cmd **cmd, char **path, int *exit_status)
{
	if (!path)
	{
		if (!is_full_path(cmd, exit_status))
			error_command_not_found(cmd, exit_status);
	}
}

static	void	cmd_not_found(t_cmd **cmd, int *exit_status)
{
	if (!(*cmd)->cmd || is_builtin((*cmd)->cmd))
		return ;
	if (!ft_strnstr((*cmd)->cmd, "/", ft_strlen((*cmd)->cmd)))
			error_command_not_found(cmd, exit_status);
}

static	int	cmd_for_path(t_cmd **cmd, char *path, int *exit_status)
{
	char	*tmp;
	char	*ex_path;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
		exit(1); // TODO: add a proper exit struct
	ex_path = ft_strjoin(tmp, (*cmd)->cmd);
	if (!ex_path)
	{
		free(tmp);
		exit(1); // TODO: add a proper exit struct
	}
	free(tmp);
	if (access(ex_path, F_OK) == 0 && access(ex_path, X_OK) == 0)
	{
		free((*cmd)->cmd);
		(*cmd)->cmd = ex_path;
		(*cmd)->is_valid_cmd = true;
		return (1);
	}
	else if (access(ex_path, F_OK) == 0)
		return (free(ex_path), error_permissions(cmd, exit_status));
	free(ex_path);
	return (0);
}

int get_cmd_path(t_cmd **cmd, char **path, int *exit_status)
{
	*exit_status = 0;
	cmd_no_path(cmd, path, exit_status);
	if ((*cmd)->cmd && !is_full_path(cmd, exit_status))
	{
		while (path && *path)
		{
			if (cmd_for_path(cmd, *path, exit_status))
				break;
			path++;
		}
	}
	cmd_not_found(cmd, exit_status);
	return 1;
}
