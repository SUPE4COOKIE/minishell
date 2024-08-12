/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 15:02:45 by scrumier          #+#    #+#             */
/*   Updated: 2024/07/22 15:12:11 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_default_fd(t_minishell *mshell)
{
	if (dup2(mshell->original_stdout, STDOUT_FILENO) != -1)
		close(mshell->original_stdout);
	else
		return ;
	if (dup2(mshell->original_stdin, STDIN_FILENO) != -1)
		close(mshell->original_stdin);
}

int	set_default_fd(t_minishell *mshell, int old[2], int new[2])
{
	mshell->original_stdout = dup(STDOUT_FILENO);
	mshell->original_stdin = dup(STDIN_FILENO);
	if (mshell->original_stdout == -1 || mshell->original_stdin == -1)
	{
		if (old[0] != -1)
			close(old[0]);
		if (old[1] != -1)
			close(old[1]);
		if (new[0] != -1)
			close(new[0]);
		if (new[1] != -1)
			close(new[1]);
		if (mshell->original_stdout != -1)
			close(mshell->original_stdout);
		if (mshell->original_stdin != -1)
			close(mshell->original_stdin);
		perror("dup failed");
		return (1);
	}
	return (0);
}

void	reset_fds(t_minishell *mshell, int old[2], int new[2])
{
	if (old[0] != -1)
		close(old[0]);
	if (old[1] != -1)
		close(old[1]);
	if (new[0] != -1)
		close(new[0]);
	if (new[1] != -1)
		close(new[1]);
	reset_default_fd(mshell);
}
