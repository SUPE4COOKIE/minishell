/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 16:51:54 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/12 12:57:36 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_in_env(char **env, char *key)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_varlen(key, '=');
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, len) && env[i][len] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	put_in_env(char **args, size_t i, t_minishell *mshell)
{
	char	*key;
	char	*value;

	key = ft_substr(args[i], 0, ft_varlen(args[i], '='));
	if (!key)
		return (error_msg(": ft_substr failed"));
	value = ft_strdup(args[i] + ft_varlen(args[i], '=') + 1);
	if (!value)
	{
		free_null(key);
		return (error_msg(": ft_strdup failed"));
	}
	if (is_in_env(mshell->env, key))
		set_env(&mshell->env, key, value);
	else
		ft_addenv(mshell, key, value);
	if (strncmp(key, "PATH=", 5) == 0)
	{
		free_tab(mshell->path);
		if (parse_path(mshell, value))
		{
			free(key);
			free(value);
			return (error_msg(": parse_path failed"));
		}
	}
	return (EXIT_SUCCESS);
}

size_t	ft_varlen(char *var, char c)
{
	size_t	i;

	i = 0;
	if (!var)
		return (0);
	while (var[i] && var[i] != c)
		i++;
	return (i);
}

bool	update_existing_env(char ***env, char *key, char *value, int index)
{
	char	*new_env;
	char	*tmp;

	new_env = ft_strjoin(key, "=");
	if (!new_env)
		return (EXIT_FAILURE);
	tmp = ft_strjoin(new_env, value);
	if (!tmp)
	{
		free_null(new_env);
		return (EXIT_FAILURE);
	}
	free_null(new_env);
	free_null((*env)[index]);
	(*env)[index] = tmp;
	return (EXIT_SUCCESS);
}

bool	set_env(char ***env, char *key, char *value)
{
	int	i;

	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], key, ft_strlen(key)) == 0 && \
				(*env)[i][ft_strlen(key)] == '=')
			return (update_existing_env(env, key, value, i));
		i++;
	}
	return (add_new_env_var(env, key, value, i));
}
