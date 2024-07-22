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
	printf("STDOUT = %d\n", STDOUT_FILENO);
	printf("STDIN = %d\n", mshell->original_stdin);
	if (dup2(mshell->original_stdout, STDOUT_FILENO) == -1)
		exit(free_shell(mshell, errno));
	close(mshell->original_stdout);
	if (dup2(mshell->original_stdin, STDIN_FILENO) == -1)
		exit(free_shell(mshell, errno));
	close(mshell->original_stdin);
}

void	set_default_fd(t_minishell *mshell, int old[2], int new[2])
{
	mshell->original_stdout = dup(STDOUT_FILENO);
	mshell->original_stdin = dup(STDIN_FILENO);
	if (mshell->original_stdout == -1 || mshell->original_stdin == -1)
	{
		if (old[0])
			close(old[0]);
		if (old[1])
			close(old[1]);
		if (new[0])
			close(new[0]);
		if (new[1])
			close(new[1]);
		perror("dup failed");
		exit(free_shell(mshell, errno));
	}
}

void	reset_fds(t_minishell *mshell, int old[2], int new[2])
{
	if (old[0])
		close(old[0]);
	if (old[1])
		close(old[1]);
	if (new[0])
		close(new[0]);
	if (new[1])
		close(new[1]);
	set_default_fd(mshell, old, new);
}
