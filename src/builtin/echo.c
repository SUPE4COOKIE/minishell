/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:36:02 by scrumier          #+#    #+#             */
/*   Updated: 2024/05/28 18:54:37 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** @brief Check if the argument is -n
** @param arg The argument
** @return true if the argument is -n, false otherwise
*/
static bool	is_n(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] != '-')
		return (false);
	i++;
	while (arg[i] && arg[i] == 'n')
		i++;
	if (arg[i])
		return (false);
	return (true);
}

/*
** @brief Print the arguments
** @param args The arguments
** @param i The index
** @param n The n flag
** @return void
*/
static void	print_args(char **args, int i, bool n)
{
	if (!args[i])
	{
		if (!n)
			ft_putchar_fd('\n', STDOUT_FILENO);
		return ;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (!n)
		ft_putchar_fd('\n', STDOUT_FILENO);
}

/*
** @brief The echo builtin
** @param mshell The minishell structure
** @param args The arguments
** @return 0 if success, 1 if error
*/
int	builtin_echo(t_minishell *mshell, char **args)
{
	int		i;
	bool	n;

	(void)mshell;
	i = 1;
	n = false;
	if (args[i] && is_n(args[i]))
	{
		n = true;
		i++;
	}
	print_args(args, i, n);
	return (EXIT_SUCCESS);
}
