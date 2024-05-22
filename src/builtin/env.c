/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sonamcrumiere <sonamcrumiere@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 14:23:07 by scrumier          #+#    #+#             */
/*   Updated: 2024/05/19 14:56:34 by sonamcrumie      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** @brief Print the environment variables
** @param mshell The minishell structure
** @param args The arguments of the command
** @return The exit status of the command (127 if too many arguments)
*/
int	builtin_env(t_minishell *mshell, char **args)
{
	int	i;

	if (args && args[1])
	{
		ft_putstr_fd("minishell: env: too many arguments\n", STDERR_FILENO);
		return (127);
	}
	i = 0;
	if (!mshell->env)
		return (EXIT_SUCCESS);
	while (mshell->env[i])
	{
		ft_putstr_fd(mshell->env[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
	return (EXIT_SUCCESS);
}
