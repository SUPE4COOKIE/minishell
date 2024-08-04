/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:40:14 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/04 14:03:39 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void allocate_env(t_minishell *mshell, char **envp)
{
	size_t i;
	size_t len;

	i = 0;
	while (envp[i])
		i++;
	mshell->env = malloc(sizeof(char *) * (i + 1));
	if (!mshell->env)
		exit(1);
	i = 0;
	while (envp[i])
	{
		len = ft_strlen(envp[i]);
		mshell->env[i] = malloc(sizeof(char) * (len + 1));
		if (!mshell->env[i])
			exit(1);
		ft_strlcpy(mshell->env[i], envp[i], len + 1);
		i++;
	}
	mshell->env[i] = NULL;
}

void	free_env_path(t_minishell *mshell)
{
	size_t i;

	i = 0;
	if (mshell->env)
	{
		while (mshell->env[i])
		{
			free(mshell->env[i]);
			i++;
		}
		free(mshell->env);
	}
	i = 0;
	if (mshell->path)
	{
		while (mshell->path[i])
		{
			free(mshell->path[i]);
			i++;
		}
		free(mshell->path);
	}
}

static int	parse_path(t_minishell *mshell, char *env)
{
	char	*path;

	path = ft_strdup(env + 5);
	if (!path)
		return (1); // TODO: add a free struct
	mshell->path = ft_split(path, ':');
	free(path);
	if (!mshell->path)
		return (1); // TODO: add a free struct
	return (0);
}

static int	set_sysbin(t_minishell *mshell)
{
	mshell->path = ft_split(DEFAULT_PATH, ':');
	if (!mshell->path)
		return (1); // TODO: add a free struct
	return (0);
}

int	save_path(t_minishell *mshell, char **env)
{
	size_t	i;

	i = 0;
	if (!env || !env[0])
		set_sysbin(mshell);
	else
	{
		while (env[i])
		{
			if (ft_strncmp(env[i], "PATH=", 5) == 0)
			{
				if (env[i][5] == 0)
					set_sysbin(mshell);
				else
					parse_path(mshell, env[i]);
				return (0);
			}
			i++;
		}
	}
	if (!mshell->path)
		set_sysbin(mshell);
	return (0);
}