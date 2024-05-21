/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:12:57 by scrumier          #+#    #+#             */
/*   Updated: 2024/05/21 16:11:47 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** @brief: Print the current working directory
** @param mshell The minishell structure
** @param args The arguments of the command
** @return The exit status of the command
*/
int	builtin_pwd(t_minishell *mshell, char **args)
{
	char	*pwd;
	char	buffer[PATH_MAX];

	if (args && args[1])
		return (error_cmd(mshell, 1, "pwd: too many arguments"));
	if (mshell->pwd)
	{
		ft_putendl_fd(mshell->pwd, STDOUT_FILENO);
		return (EXIT_SUCCESS);
	}
	pwd = getcwd(buffer, PATH_MAX);
	if (!pwd)
		return (error_cmd(mshell, 1, "pwd: error retrieving current directory: getcwd failed"));
	else
		ft_putendl_fd(pwd, STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
