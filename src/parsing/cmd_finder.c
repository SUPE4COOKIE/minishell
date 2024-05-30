/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_finder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 20:55:03 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/05/30 14:38:44 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int is_full_path(t_cmd **cmd)
{
	char *tmp;

	if (ft_strnstr((*cmd)->cmd, "/", ft_strlen((*cmd)->cmd)))
	{
		if (access((*cmd)->cmd, F_OK) == 0 && access((*cmd)->cmd, X_OK) == 0)
		{
			(*cmd)->is_valid_cmd = true;
			return (1);
		}
		else
		{
			tmp = ft_strjoin((*cmd)->cmd, ": command not found\n");
			if (!tmp)
				exit(1); // TODO: add a proper exit struct
			write(2, tmp, ft_strlen(tmp));
			free(tmp);
			free((*cmd)->cmd);
			(*cmd)->is_valid_cmd = false;
			return (2);
		}
	}
	return (0);
}

static void cmd_no_path(t_cmd **cmd, char **path)
{
	char *tmp;

	if (!path)
	{
		if (!is_full_path(cmd))
		{
			tmp = ft_strjoin((*cmd)->cmd, ": command not found\n");
			if (!tmp)
				exit(1); // TODO: add a proper exit struct
			write(2, tmp, ft_strlen(tmp));
			free(tmp);
			free((*cmd)->cmd);
			(*cmd)->is_valid_cmd = false;
		}
	}
}
static	void	cmd_not_found(t_cmd **cmd)
{
	char	*tmp;

	if ((*cmd)->cmd && (access((*cmd)->cmd , F_OK) != 0 \
		|| access((*cmd)->cmd , X_OK) != 0) && (*cmd)->cmd)
	{
		tmp = ft_strjoin((*cmd)->cmd , ": command not found\n");
		if (!tmp)
			exit(1); // TODO: add a proper exit struct
		write(2, tmp, ft_strlen(tmp));
		free(tmp);
		free((*cmd)->cmd);
		(*cmd)->is_valid_cmd = false;
		(*cmd)->cmd = NULL;
	}
}

static	int	cmd_for_path(t_cmd **cmd, char *path)
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
	free(ex_path);
	return (0);
}

int get_cmd_path(t_cmd **cmd, char **path)
{
	cmd_no_path(cmd, path);
	if ((*cmd)->cmd && !is_full_path(cmd))
	{
		while (path && *path)
		{
			if (cmd_for_path(cmd, *path))
				break;
			path++;
		}
	}
	cmd_not_found(cmd);
	return 1;
}