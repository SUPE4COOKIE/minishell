/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:12:57 by scrumier          #+#    #+#             */
/*   Updated: 2024/05/28 18:36:55 by scrumier         ###   ########.fr       */
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
	char	buffer[PATH_MAX];

	if (args && args[1])
		return (error_cmd(mshell, 1, "pwd: too many arguments"));
	getcwd(buffer, PATH_MAX);
	ft_putendl_fd(buffer, STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
