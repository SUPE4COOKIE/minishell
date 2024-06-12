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

bool is_in_env(char *arg, char **env)
{
	size_t	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(arg, env[i], ft_strlen(arg)) == 0 && env[i][ft_strlen(arg)] == '=')
			return (true);
		i++;
	}
	return (false);
}

void	ft_addenv(t_minishell *mshell, char *new_arg)
{
	char	**new_env;
	size_t	i;

	i = 0;
	while (mshell->env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return ;
	i = 0;
	while (mshell->env[i])
	{
		new_env[i] = ft_strdup(mshell->env[i]);
		if (!new_env[i])
		{
			free_tab(new_env);
			return ;
		}
		i++;
	}
	new_env[i] = ft_strdup(new_arg);
	if (!new_env[i])
	{
		free_tab(new_env);
		return ;
	}
	new_env[i + 1] = NULL;
	//free_tab(mshell->env);
	mshell->env = new_env;
	return ;
}

void put_in_env(char **args, size_t i, t_minishell *mshell)
{
	if (is_in_env(args[i], mshell->env))
		set_env(&mshell->env, args[i] + ft_strlen(args[i]) + 1, args[i]);
	else
		ft_addenv(mshell, args[i]);
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
		// print l'env mais c'est chelou
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
	print_tab(mshell->env);
	return (0);
}
