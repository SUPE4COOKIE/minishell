/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 16:27:48 by scrumier          #+#    #+#             */
/*   Updated: 2024/07/22 16:28:21 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_infiles(t_cmd *cmd, t_minishell *mshell)
{
	int	i;

	i = 0;
	while (cmd->infile && cmd->infile[i])
	{
		if (process_infile(cmd, mshell, i) == 1)
			return (1);
		i++;
	}
	return (0);
}

int	check_outfiles(t_cmd *cmd, t_minishell *mshell, int i)
{
	if (cmd->outfile[i])
	{
		if (process_outfile(cmd, mshell, i) == 1)
			return (1);
		i++;
	}
	return (0);
}
