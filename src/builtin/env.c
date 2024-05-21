/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 14:23:07 by scrumier          #+#    #+#             */
/*   Updated: 2024/05/21 13:10:42 by scrumier         ###   ########.fr       */
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
		return (125);
	}
	i = 0;
	if (!mshell->env)
		return (EXIT_SUCCESS);
	while (mshell->env[i])
	{
		ft_putendl_fd(mshell->env[i], STDOUT_FILENO);
		i++;
	}
	return (EXIT_SUCCESS);
}
