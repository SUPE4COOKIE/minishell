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

bool is_in_env(char *key, char **env)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(key);
	if (!env)
		return (false);
	while (env[i])
	{
		if (!ft_strncmp(key, env[i], len) && env[i][len] == '=')
			return (true);
		i++;
	}
	return (false);
}

char **ft_addenv(char *new_env, char **env)
{
	size_t	i;
	char	**new_env_tab;

	i = 0;
	while (env[i])
		i++;
	new_env_tab = malloc(sizeof(char *) * (i + 2));
	if (!new_env_tab)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env_tab[i] = ft_strdup(env[i]);
		i++;
	}
	new_env_tab[i] = ft_strdup(new_env);
	new_env_tab[i + 1] = NULL;
	free_tab(env);
	return (new_env_tab);
}

void put_in_env(char **args, size_t i, t_minishell *mshell)
{
	char	*key;
	char	*value;
	char	*tmp;

	key = ft_strdup(args[i]);
	if (!args[i + 1])
		value = ft_strdup("");
	else
		value = ft_strdup(args[i + 1]);
	tmp = ft_strjoin(key, "=");
	free(key);
	key = tmp;
	tmp = ft_strjoin(key, value);
	free(key);
	free(value);
	key = tmp;
	if (is_in_env(key, mshell->env))
		set_env(&mshell->env, value, key);
	else
		mshell->env = ft_addenv(key, mshell->env);
	free(key);
}

int check_input(char *args)
{
	size_t	i;

	i = 0;
	if (!args || !args[0] || args[0] == '=')
		return (0);
	if (!ft_isalpha(args[0]))
		return (0);
	while (args[i])
	{
		if (!ft_isalnum(args[i]) && args[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_export(t_minishell *mshell, char **args)
{
	size_t	i;

	if (!args || !args[1])
	{
		// print l'env mais c'est chelou
		return (1);
	}
	i = 1;
	while (args[i])
	{
		if (!check_input(args[i]))
		{
			printf("minishell: export: `%s': not a valid identifier\n", args[i]);
			continue ;
		}
		put_in_env(args, i, mshell);
		i++;
	}
	return (0);
}
