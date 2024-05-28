/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:08:17 by scrumier          #+#    #+#             */
/*   Updated: 2024/05/28 18:42:56 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_tablen(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

char **remove_env_var(char **env, int index)
{
	char	**new_env;
	int		i;
	int		j;

	new_env = malloc(sizeof(char *) * (ft_tablen(env)));
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (env[i])
	{
		if (i != index)
		{
			new_env[j] = ft_strdup(env[i]);
			j++;
		}
		free(env[i]);
		i++;
	}
	new_env[j] = NULL;
	free(env);
	return (new_env);
}

bool is_valid_env_var(char *var)
{
	int	i;

	i = 0;
	if (ft_isdigit(var[0]))
		return (false);
	while (var[i])
	{
		if (ft_isalnum(var[i]) == false && var[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

int get_index_env(char **env, char *var)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(var);
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	builtin_unset(t_minishell *mshell, char **args)
{
	int	i;
	int	ret;
	int	index;

	ret = EXIT_SUCCESS;
	if (!args || !args[1] || !mshell->env || !mshell->env[0])
		return (EXIT_SUCCESS);
	i = 1;
	while (args[i])
	{
		index = get_index_env(mshell->env, args[i]);
		if (index != -1)
			mshell->env = remove_env_var(mshell->env, index);
		i++;
	}
	return (EXIT_SUCCESS);
}
