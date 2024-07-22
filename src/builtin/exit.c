/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:04:04 by scrumier          #+#    #+#             */
/*   Updated: 2024/07/22 14:35:20 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
** @brief: Free all allocated memory and exit the shell
** @param mshell The minishell structure
** @param status The exit status
*/
int	free_shell(t_minishell *mshell, int status)
{
	free_mshell(mshell);
	return (status);
}

/**
** @brief: Check if the command is between two commands
** @param mshell The minishell structure
** @return true if the command is between two commands, false otherwise
*/
static bool	get_is_between_cmd(t_minishell *mshell)
{
	t_cmd	*cmd;

	cmd = mshell->cmds;
	if (cmd)
		if (cmd->next)
			return (true);
	return (false);
}

long ft_atol_overflow(const char *str, bool *is_overflow)
{
	long int result;
	int sign;

	sign = 1;
	result = 0;
	if (*str == '\0')
		return (0);
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	while (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -sign;
		str++;
	}
	while (ft_isdigit(*str))
	{
		if (sign == 1 && (result > (INT_MAX - (*str - '0')) / 10))
		{
			*is_overflow = true;
			return (2);
		}
		if (sign == -1 && (-result < (INT_MIN + (*str - '0')) / 10))
		{
			*is_overflow = true;
			return (2);
		}
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

uint8_t atoutint8(const char *str)
{
	long int	result;
	bool		is_overflow;

	is_overflow = false;
	result = ft_atol_overflow(str, &is_overflow);
	if (is_overflow == true)
		ft_putendl_fd("Exit : Numeric argument required", 2);
	return (result);
}


/**
** @brief: Exit the shell
** @param mshell The minishell structure
** @param args The arguments
** @return Exit status
*/
int	builtin_exit(t_minishell *mshell, char **args)
{
	if (get_is_between_cmd(mshell) == false)
		ft_putstr_fd("exit\n", 2);
	else
		return (0);
	if (args)
	{
		if (args[1] && isnumber(args[1]) == false)
		{
			mshell->last_exit_status = 2;
			ft_putendl_fd("Exit : numeric argument required", 2);
		}
		else if (args[1])
			mshell->last_exit_status = atoutint8(args[1]);
		if (args[1] && args[2])
			return (error_cmd(mshell,1, "exit: too many arguments"));
		exit(free_shell(mshell, mshell->last_exit_status));
	}
	return (0);
}
