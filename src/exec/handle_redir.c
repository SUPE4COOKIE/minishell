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

/**
 * @brief Handle the redirection of the standard output
 * @param cmd The command
 * @param old The old file descriptors
 * @param new The new file descriptors
 */
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

/**
 * @brief Handle the redirection of the standard input
 * @param cmd The command
 * @param old The old file descriptors
 * @param new The new file descriptors
 */
void	handle_red_in(t_cmd *cmd, int old[2], int new[2])
{
	int	fd;
	int i;

	i = 0;
	while (cmd->infile[i + 1])
		i++;
	fd = open(cmd->infile[i], O_RDONLY);
	if (fd == -1)
		error_pipe("open failed", new, old, cmd);
	if (dup2(fd, STDIN_FILENO) == -1)
		error_pipe("dup2 failed", new, old, cmd);
	close(fd);
}

/**
 * @brief Handle the append of the standard output
 * @param cmd The command
 * @param old The old file descriptors
 * @param new The new file descriptors
 */
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

int check_infiles(t_cmd *cmd)
{
	struct stat	buf;
	char		*tmp;
	int			i = 0;
	int			access_status;

	while (cmd->infile[i])
	{
		if (stat(cmd->infile[i], &buf) == -1)
			return (perror(cmd->infile[i]), 1);
		if (((buf.st_mode) & 0170000) == (0040000))
		{
			tmp = ft_strjoin(cmd->infile[i], ": is a directory\n");
			if (!tmp)
				exit(1); // TODO: add a proper exit struct
			write(2, tmp, ft_strlen(tmp));
			free(tmp);
			return (1);
		}
		access_status = access(cmd->infile[i], R_OK);
		if (access_status == -1)
			return (perror(cmd->infile[i]), 1);
		i++;
	}
	return (0);
}

int	check_outfiles(t_cmd *cmd)
{
	struct stat	buf;
	char		*tmp;
	int			i;
	int			access_status;

	i = 0;
	while (cmd->outfile[i])
	{
		if (stat(cmd->outfile[i], &buf) == -1)
			return (perror(cmd->outfile[i]), 1);
		if (((buf.st_mode) & 0170000) == (0040000))
		{
			tmp = ft_strjoin(cmd->outfile[i], ": is a directory\n");
			if (!tmp)
				exit(1); // TODO: add a proper exit struct
			write(2, tmp, ft_strlen(tmp));
			free(tmp);
			return (1);
		}
		access_status = access(cmd->outfile[i], W_OK);
		if (access_status == -1)
			return (perror(cmd->outfile[i]), 1);
		i++;
	}
	return (0);
}


/**
 * @brief Handle the file redirection
 * @param cmd The command
 * @param old The old file descriptors
 * @param new The new file descriptors
 */
void	handle_file_redirection(t_minishell *mshell, t_cmd *cmd, int old[2], int new[2])
{
	int	fd;
	int	i;

	if (cmd && (cmd->infile || cmd->outfile))
	{
		i = 0;
		if (cmd->infile && cmd->infile[i])
		{
			if (check_infiles(cmd) == 1)
			{
				cmd->is_valid_cmd = false;
				mshell->last_exit_status = 1;
				return ;
			}
			if (cmd->op_type[0] == RED_IN)
				handle_red_in(cmd, old, new);
		}
		if (cmd->outfile && cmd->outfile[i])
		{
			if (cmd->outfile && cmd->outfile[i])
			{
				while (cmd->outfile[i + 1]) {
					fd = open(cmd->outfile[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
					if (fd == -1)
						error_pipe("open failed", new, old, cmd);
					close(fd);
					i++;
				}
			}
			if (check_outfiles(cmd) == 1)
			{
				mshell->last_exit_status = 1;
				return ;
			}
			if (cmd->op_type[1] == RED_OUT)
				handle_red_out(cmd, old, new);
			else if (cmd->op_type[1] == APP_OUT)
				handle_append_out(cmd, old, new);
		}
	}
}
