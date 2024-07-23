/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 10:23:57 by sonamcrumie       #+#    #+#             */
/*   Updated: 2024/07/22 17:16:50 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	calculate_new_env_size(char **env)
{
	size_t	size;

	size = 0;
	while (env[size])
		size++;
	return (size);
}

char	**duplicate_env(t_minishell *mshell, size_t size)
{
	char	**new_env;
	size_t	i;

	new_env = ft_calloc(size + 2, sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while (mshell->env[i])
	{
		new_env[i] = ft_strdup(mshell->env[i]);
		if (!new_env[i])
			return (free_tab(new_env), NULL);
		i++;
	}
	return (new_env);
}

int	ft_addenv(t_minishell *mshell, char *key, char *value)
{
	size_t	size;
	char	**new_env;
	char	*temp;

	size = calculate_new_env_size(mshell->env);
	new_env = duplicate_env(mshell, size);
	if (!new_env)
		return (1);
	temp = ft_strjoin(key, "=");
	if (!temp)
		return (free_tab(new_env), error_cmd(mshell, 1, "malloc failed"));
	new_env[size] = ft_strjoin(temp, value);
	free_null(temp);
	if (!new_env[size])
		return (free_tab(new_env), error_cmd(mshell, 1, "malloc failed"));
	new_env[size + 1] = NULL;
	free_tab(mshell->env);
	mshell->env = new_env;
	return (0);
}

int	check_input(t_minishell *mshell, char *args)
{
	size_t	i;

	i = 0;
	if (!ft_isalpha(args[i]) && args[i] != '_')
	{
		mshell->last_exit_status = 1;
		return (1);
	}
	i++;
	while (args[i])
	{
		if (args[i] == '=')
			return (0);
		else if (!ft_isalnum(args[i]) && args[i] != '_')
		{
			mshell->last_exit_status = 1;
			return (1);
		}
		i++;
	}
	mshell->last_exit_status = 0;
	return (0);
}

int	builtin_export(t_minishell *mshell, char **args)
{
	size_t	i;

	if (!args || !args[1])
		return (1);
	i = 1;
	while (args[i])
	{
		if (check_input(mshell, args[i]))
		{
			ft_putendl_fd("minishell: export: ", 2);
			ft_putendl_fd(args[i], 2);
			ft_putendl_fd("not a valid identifier\n", 2);
			mshell->last_exit_status = 1;
		}
		else
		{
			put_in_env(args, i, mshell);
			mshell->last_exit_status = 0;
		}
		i++;
	}
	return (0);
}
