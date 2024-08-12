/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:08:17 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/12 14:32:26 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_new_env(char **new_env, int count)
{
	while (count > 0)
	{
		free_null(new_env[count]);
		count--;
	}
	free_null(new_env);
}

char	**allocate_and_copy_env(char **env, int index)
{
	char	**new_env;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_env = malloc(sizeof(char *) * ft_tablen(env));
	if (!new_env)
		return (NULL);
	while (env[i])
	{
		if (i != index)
		{
			new_env[j] = ft_strdup(env[i]);
			if (!new_env[j])
				return (free_new_env(new_env, j), NULL);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	return (new_env);
}

char	**remove_env_var(char **env, int index)
{
	char	**new_env;
	int		i;

	i = 0;
	if (!env || index < 0)
		return (NULL);
	new_env = allocate_and_copy_env(env, index);
	if (!new_env)
		return (NULL);
	while (env[i])
		free_null(env[i++]);
	free_null(env);
	return (new_env);
}

/**
** @brief Get the index of an environment variable
** @param env The environment
** @param var The variable
** @return The index of the variable in the environment
*/
int	get_index_env(char **env, char *var)
{
	char	*tmp;
	int		i;

	if (!env || !var)
		return (-1);
	tmp = (char *)malloc(sizeof(char) * (ft_strlen(var) + 2));
	if (!tmp)
		return (-1);
	ft_strlcpy(tmp, var, ft_strlen(var) + 1);
	ft_strlcat(tmp, "=", ft_strlen(var) + 2);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(tmp, env[i], ft_strlen(tmp)) == 0)
		{
			free_null(tmp);
			return (i);
		}
		i++;
	}
	free_null(tmp);
	return (-1);
}

/**
** @brief Unset the environment variables
** @param mshell The minishell structure
** @param args The arguments of the command
*/
int	builtin_unset(t_minishell *mshell, char **args)
{
	int		i;
	int		index;
	char	**new_env;

	if (!args || !args[1] || !mshell->env || !mshell->env[0])
		return (0);
	i = 1;
	while (args[i])
	{
		index = get_index_env(mshell->env, args[i]);
		if (index != -1)
		{
			new_env = remove_env_var(mshell->env, index);
			if (!new_env)
				return (1);
			mshell->env = new_env;
		}
		i++;
	}
	return (0);
}
