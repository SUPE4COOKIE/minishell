/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 14:23:07 by scrumier          #+#    #+#             */
/*   Updated: 2024/05/17 15:24:31 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
