/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:40:14 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/10 21:00:50 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	allocate_env(t_minishell *mshell, char **envp)
{
	size_t	i;
	size_t	len;

	i = 0;
	while (envp[i])
		i++;
	mshell->env = ft_calloc(i + 1, sizeof(char *));
	if (!mshell->env)
		return (1);
	i = 0;
	while (envp[i])
	{
		len = ft_strlen(envp[i]);
		mshell->env[i] = malloc(sizeof(char) * (len + 1));
		if (!mshell->env[i])
			return (free_str_array(mshell->env), 1);
		ft_strlcpy(mshell->env[i], envp[i], len + 1);
		i++;
	}
	mshell->env[i] = NULL;
	return (0);
}

void	free_env_path(t_minishell *mshell)
{
	size_t	i;

	i = 0;
	if (mshell->env)
	{
		while (mshell->env[i])
		{
			free_null(mshell->env[i]);
			i++;
		}
		free_null(mshell->env);
	}
	i = 0;
	if (mshell->path)
	{
		while (mshell->path[i])
		{
			free_null(mshell->path[i]);
			i++;
		}
		free_null(mshell->path);
	}
}

int	parse_path(t_minishell *mshell, char *env)
{
	char	*path;

	path = ft_strdup(env + 5);
	if (!path)
		return (1);
	mshell->path = ft_split(path, ':');
	free_null(path);
	if (!mshell->path)
		return (1);
	return (0);
}

int	env_path_saver(t_minishell *mshell, char **env)
{
	size_t	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			if (env[i][5] == 0)
				return (set_sysbin(mshell));
			else
				return (parse_path(mshell, env[i]));
		}
		i++;
	}
	return (0);
}

int	save_path(t_minishell *mshell, char **env)
{
	if (!env || !env[0])
	{
		if (set_sysbin(mshell))
			return (1);
	}
	else
	{
		if (env_path_saver(mshell, env))
			return (1);
	}
	if (!mshell->path)
		return (set_sysbin(mshell));
	return (0);
}
