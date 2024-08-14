/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:03:38 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/14 11:05:02 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

size_t	calculate_new_env_size(char **env)
{
	size_t	size;

	size = 0;
	while (env[size])
		size++;
	return (size);
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
