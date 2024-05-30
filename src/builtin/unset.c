/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:08:17 by scrumier          #+#    #+#             */
/*   Updated: 2024/05/30 15:11:18 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** @brief Get the length of a table
** @param tab The table
** @return The length of the table
*/
int ft_tablen(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return (0);
	while (tab[i])
		i++;
	return (i);
}

/*
** @brief Remove an environment variable
** @param env The environment
** @param index The index of the variable to remove
** @return The new environment without the variable
*/
char **remove_env_var(char **env, int index)
{
	char	**new_env;
	int		i;
	int		j;

	if (!env || index < 0)
		return (NULL);
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
			if (!new_env[j])
			{
				while (i-- > 0)
					free(new_env[i]);
				free(new_env);
				return (NULL);
			}
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
	return (new_env);
}

/*
** @brief Check if the variable is a valid environment variable
** @param var The variable
** @return true if the variable is valid, false otherwise
*/
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

/*
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
			free(tmp);
			return (i);
		}
		i++;
	}
	free(tmp);
	return (-1);
}

/*
** @brief Unset the environment variables
** @param mshell The minishell structure
** @param args The arguments of the command
*/
void	builtin_unset(t_minishell *mshell, char **args)
{
	int	i;
	int	index;
	char **new_env;

	if (!args || !args[1] || !mshell->env || !mshell->env[0])
		return ;
	i = 1;
	while (args[i])
	{
		index = get_index_env(mshell->env, args[i]);
		if (index != -1)
		{
			new_env = remove_env_var(mshell->env, index);
			if (!new_env)
				return ;
			mshell->env = new_env;
		}
		i++;
	}
}
