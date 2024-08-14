/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 10:23:57 by sonamcrumie       #+#    #+#             */
/*   Updated: 2024/08/14 11:15:32 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_input(t_minishell *mshell, char *args)
{
	size_t	i;

	i = 0;
	if (!ft_isalpha(args[i]) && args[i] != '_')
	{
		mshell->last_exit_status = 1;
		return (1);
	}
	i++;
	while (args[i])
	{
		if (args[i] == '=')
			return (0);
		else if (!ft_isalnum(args[i]) && args[i] != '_')
		{
			mshell->last_exit_status = 1;
			return (1);
		}
		i++;
	}
	mshell->last_exit_status = 0;
	return (0);
}

int	builtin_export(t_minishell *mshell, char **args)
{
	size_t	i;

	if (!args || !args[1])
		return (1);
	i = 0;
	while (args[++i])
	{
		if (ft_strchr(args[i++], '=') == NULL)
			continue ;
		i--;
		if (check_input(mshell, args[i]))
		{
			ft_putendl_fd(": ", 2);
			ft_putendl_fd(args[i], 2);
			ft_putendl_fd("not a valid identifier\n", 2);
			mshell->last_exit_status = 1;
		}
		else
		{
			if (put_in_env(args, i, mshell))
				return (1);
			mshell->last_exit_status = 0;
		}
	}
	return (0);
}
