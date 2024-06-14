/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:01:08 by scrumier          #+#    #+#             */
/*   Updated: 2024/06/14 14:32:19 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_red_out(t_cmd *cmd, int old[2], int new[2])
{
	int	fd;

	fd = open(cmd->outfile[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		error_pipe("open failed", new, old, cmd);
	if (dup2(fd, STDOUT_FILENO) == -1)
		error_pipe("dup2 failed", new, old, cmd);
	close(fd);
}

void	handle_red_in(t_cmd *cmd, int old[2], int new[2])
{
	int	fd;

	fd = open(cmd->infile[0], O_RDONLY);
	if (fd == -1)
		error_pipe("open failed", new, old, cmd);
	if (dup2(fd, STDIN_FILENO) == -1)
		error_pipe("dup2 failed", new, old, cmd);
	close(fd);
}

void	handle_append_out(t_cmd *cmd, int old[2], int new[2])
{
	int	fd;

	fd = open(cmd->outfile[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		error_pipe("open failed", new, old, cmd);
	if (dup2(fd, STDOUT_FILENO) == -1)
		error_pipe("dup2 failed", new, old, cmd);
	close(fd);
}

/**
 * @brief Handle file redirection
 * @param mshell
 * @param cmd
 * @param old
 * @param new
 */
void	handle_file_redirection(t_cmd *cmd, int old[2], int new[2])
{
	int	fd;
	int	i;

	i = 0;
	if (cmd->outfile && cmd->outfile[i])
	{
		while (cmd->outfile[i + 1])
		{
			fd = open(cmd->outfile[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				error_pipe("open failed", new, old, cmd);
			close(fd);
			i++;
		}
	}
	if (cmd->op_type[1] == RED_OUT)
		handle_red_out(cmd, old, new);
	else if (cmd->op_type[1] == APP_OUT)
		handle_append_out(cmd, old, new);
	else if (cmd->op_type[0] == RED_IN)
		handle_red_in(cmd, old, new);
}
