/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 10:23:57 by sonamcrumie       #+#    #+#             */
/*   Updated: 2024/05/28 18:54:43 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return (0);
	while (env[i])
		i++;
	return (i);
}

int	builtin_export(t_minishell *mshell, const char *name, \
		const char *value, int overwrite)
{
	t_setenv	setenv;

	(void)value;
	(void)overwrite;
	(void)mshell;
	(void)name;
	(void)setenv;
	return (EXIT_SUCCESS);
}
