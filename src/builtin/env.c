/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 14:23:07 by scrumier          #+#    #+#             */
/*   Updated: 2024/05/17 14:39:07 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// A mon avis il marche pas (enfin ca me parait trop court pour etre correct)

int	builtin_env(t_minishell *mshell, char **args)
{
	int	i;

	if (args && args[1])
	{
		ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	i = 0;
	if (!mshell->env)
		return (EXIT_FAILURE);
	while (mshell->env[i])
	{
		ft_putstr_fd(mshell->env[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
	return (EXIT_SUCCESS);
}
