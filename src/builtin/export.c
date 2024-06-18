/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 10:23:57 by sonamcrumie       #+#    #+#             */
/*   Updated: 2024/05/30 15:08:22 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t ft_varlen(char *var, char c)
{
	size_t	i;

	i = 0;
	while (var[i] && var[i] != c)
		i++;
	return (i);
}

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

void	ft_addenv(t_minishell *mshell, char *key, char *value)
{
	size_t	i;
	char	**new_env;

	i = 0;
	while (mshell->env[i])
		i++;
	new_env = ft_calloc(i + 2, sizeof(char *));
	if (!new_env)
		error_cmd(mshell, 1, "malloc failed");
	i = 0;
	while (mshell->env[i])
	{
		new_env[i] = ft_strdup(mshell->env[i]);
		if (!new_env[i])
			error_cmd(mshell, 1, "malloc failed");
		i++;
	}
	new_env[i] = ft_strjoin(key, "=");
	if (!new_env[i])
		error_cmd(mshell, 1, "malloc failed");
	new_env[i] = ft_strjoin(new_env[i], value);
	if (!new_env[i])
		error_cmd(mshell, 1, "malloc failed");
	new_env[i + 1] = NULL;
	free_tab(mshell->env);
	mshell->env = new_env;
}

void put_in_env(char **args, size_t i, t_minishell *mshell)
{
	char *key;
	char *value;

	key = ft_substr(args[i], 0, ft_varlen(args[i], '='));
	value = ft_strdup(args[i] + ft_varlen(args[i], '=') + 1);
	if (is_in_env(mshell->env, key))
		set_env(&mshell->env, value, key);
	else
		ft_addenv(mshell, key, value);
}

int check_input(char *args)
{
	size_t	i;

	i = 0;
	if (!ft_isalpha(args[i]) && args[i] != '_')
		return (1);
	i++;
	while (args[i])
	{
		if (args[i] == '=')
			return (0);
		else if (!ft_isalnum(args[i]) && args[i] != '_')
			return (1);
		i++;
	}
	return (1);
}

int	builtin_export(t_minishell *mshell, char **args)
{
	size_t	i;

	if (!args || !args[1])
	{
		// print the environment
		return (1);
	}
	i = 1;
	while (args[i])
	{
		if (check_input(args[i]))
		{
			printf("minishell: export: `%s': not a valid identifier\n", args[i]);
			break ;
		}
		else
			put_in_env(args, i, mshell);
		i++;
	}
	return (0);
}
