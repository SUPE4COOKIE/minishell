/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 15:55:11 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/16 11:38:41 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_red_out(t_cmd *cmd, t_minishell *mshell)
{
	int	fd;
	int	i;

	i = 0;
	while (cmd->outfile[i] && cmd->outfile[i + 1])
		i++;
	if (mshell->invalid_redir != NULL && is_redir_before(cmd, \
			&mshell->invalid_redir, &cmd->outfile[i]) == true)
	{
		fd = open("/dev/null", O_WRONLY, 0644);
		if (fd == -1)
			return (perror("open"), 1);
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (perror("dup2"), 1);
	}
	else
	{
		fd = open(cmd->outfile[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (perror("open"), 1);
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (perror("dup2"), 1);
		close(fd);
	}
	return (0);
}

void	handle_append_out(t_cmd *cmd, int old[2], int new[2], \
		t_minishell *mshell)
{
	int	fd;
	int	i;

	i = 0;
	while (cmd->outfile[i] && cmd->outfile[i + 1])
		i++;
	if (mshell->invalid_redir != NULL && is_redir_before(cmd, \
			&mshell->invalid_redir, &cmd->outfile[i]) == true)
	{
		fd = open("/dev/null", O_WRONLY, 0644);
		if (dup2(fd, STDOUT_FILENO) == -1)
			error_pipe("dup2 failed", new, old, cmd);
	}
	fd = open(cmd->outfile[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (perror("open"));
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (perror("dup2"));
	close(fd);
}

/**
 * @brief Handle the redirection of the standard input
 * @param cmd The command
 * @param old The old file descriptors
 * @param new The new file descriptors
 */
void	handle_red_in(t_cmd *cmd, t_minishell *mshell)
{
	int	fd;
	int	i;

	i = 0;
	while (cmd->infile && cmd->infile[i] && cmd->infile[i + 1])
		i++;
	if (mshell->invalid_redir != NULL && is_redir_before(cmd, \
			&mshell->invalid_redir, &cmd->outfile[i]) == true)
	{
		fd = open("/dev/null", O_WRONLY, 0644);
		if (fd == -1)
			return (perror("open"));
		if (dup2(fd, STDIN_FILENO) == -1)
			perror("dup2");
		close(fd);
		return ;
	}
	fd = open(cmd->infile[i], O_RDONLY);
	if (fd == -1)
		return (perror("open"));
	if (dup2(fd, STDIN_FILENO) == -1)
		return (close(fd), perror("dup2"));
	close(fd);
}
