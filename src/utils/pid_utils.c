/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:14:41 by scrumier          #+#    #+#             */
/*   Updated: 2024/06/14 14:39:37 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
** @brief Close the file descriptors
** @param old The old file descriptors
** @param new The new file descriptors
*/
void	ft_close(int old[2], int new[2])
{
	if (old[0] >= 0)
		close(old[0]);
	if (old[1] >= 0)
		close(old[1]);
	if (new[0] >= 0)
		close(new[0]);
	if (new[1] >= 0)
		close(new[1]);
}

/**
** @brief Initialize the old and new file descriptors
** @param old The old file descriptors
** @param new The new file descriptors
*/
int	init_exec(int old[2], int new[2], t_minishell *mshell)
{
	old[0] = -1;
	old[1] = -1;
	new[0] = -1;
	new[1] = -1;
	mshell->in_heredoc = true;
	if (replace_hdoc(mshell->cmds, old, new))
		return (1);
	mshell->in_heredoc = false;
	return (0);
}

/**
 * @brief Close the old file descriptors
 * @param i The index
 * @param old The old file descriptors
 */
void close_old(int i, int old[2])
{
	if (i > 0)
	{
		if (old[0] >= 0)
			close(old[0]);
		if (old[1] >= 0)
			close(old[1]);
	}
}