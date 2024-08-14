/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 14:23:07 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/14 11:13:51 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
** @brief Check if the character is in the string
** @param str The string
** @param c The character
** @param n The number of occurrence
** @return 1 if the character is in the string n times, 0 otherwise
*/
int	ft_contain(char *str, char c, int n)
{
	int	i;

	i = 0;
	if (n == 0 || !str || !c || !str[0])
		return (1);
	while (str[i])
	{
		if (str[i] == c)
			n--;
		i++;
	}
	return (n);
}

/**
** @brief Print the environment variables
** @param mshell The minishell structure
** @param args The arguments of the command
** @return The exit status of the command (127 if too many arguments)
*/
int	builtin_env(t_minishell *mshell, char **args)
{
	int	i;

	i = 1;
	if (args && args[i])
	{
		while (args[i])
		{
			if (ft_contain(args[i], '/', 1) == 0)
				return (error_cmd(mshell, 127, ": invalid option"));
			i++;
		}
	}
	if (!mshell->env)
		return (EXIT_SUCCESS);
	i = -1;
	while (mshell->env[++i])
		ft_putendl_fd(mshell->env[i], STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
