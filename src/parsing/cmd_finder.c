/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_finder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 20:55:03 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/07/13 12:52:15 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

static int is_full_path(t_cmd **cmd, int *exit_status)
{
	char *tmp;
	struct	stat buf;

	if (is_builtin((*cmd)->cmd))
	{
		(*cmd)->is_valid_cmd = true;
		return (1);
	}
	if (ft_strnstr((*cmd)->cmd, "/", ft_strlen((*cmd)->cmd)))
	{
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
			free_null(tmp);
			return (1);
		}
		if (access((*cmd)->cmd, F_OK) == 0 && access((*cmd)->cmd, X_OK) == 0)
		{
			(*cmd)->is_valid_cmd = true;
			return (1);
		}
		else if (access((*cmd)->cmd, F_OK) == 0 && access((*cmd)->cmd, X_OK) != 0)
		{
			*exit_status = 126;
			tmp = ft_strjoin((*cmd)->cmd, ": permission denied\n");
			if (!tmp)
				exit(1); // TODO: add a proper exit struct
			write(2, tmp, ft_strlen(tmp));
			(*cmd)->is_valid_cmd = false;
			free_null(tmp);
			return (2);
		}
		else
		{
			*exit_status = 127;
			tmp = ft_strjoin((*cmd)->cmd, ": command not found\n");
			if (!tmp)
				exit(1); // TODO: add a proper exit struct
			write(2, tmp, ft_strlen(tmp));
			(*cmd)->is_valid_cmd = false;
			free_null(tmp);
			return (2);
		}
	}
	return (0);
}

static void cmd_no_path(t_cmd **cmd, char **path, int *exit_status)
{
	char *tmp;

	if (!path)
	{
		if (!is_full_path(cmd, exit_status))
		{
			*exit_status = 127;
			tmp = ft_strjoin((*cmd)->cmd, ": command not found\n");
			if (!tmp)
				exit(1); // TODO: add a proper exit struct
			write(2, tmp, ft_strlen(tmp));
			free_null(tmp);
			(*cmd)->is_valid_cmd = false;
		}
	}
}

static	void	cmd_not_found(t_cmd **cmd, int *exit_status)
{
	char	*tmp;

	if (!(*cmd)->cmd || is_builtin((*cmd)->cmd))
		return ;
	if (!ft_strnstr((*cmd)->cmd, "/", ft_strlen((*cmd)->cmd)))
	{
			*exit_status = 127;
			tmp = ft_strjoin((*cmd)->cmd , ": command not found\n");
			if (!tmp)
				exit(1); // TODO: add a proper exit struct
			write(2, tmp, ft_strlen(tmp));
			free_null(tmp);
			free_null((*cmd)->cmd);
			(*cmd)->is_valid_cmd = false;
			(*cmd)->cmd = NULL;
	}
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
		free_null(tmp);
		exit(1); // TODO: add a proper exit struct
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
	{
		*exit_status = 126;
		tmp = ft_strjoin((*cmd)->cmd , ": permission denied\n");
		if (!tmp)
			exit(1); // TODO: add a proper exit struct
		write(2, tmp, ft_strlen(tmp));
		(*cmd)->is_valid_cmd = false;
		free_null((*cmd)->cmd);
		free_null(tmp);
		(*cmd)->cmd = NULL;
		return (1);
	}
	free_null(ex_path);
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
