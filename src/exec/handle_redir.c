/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:01:08 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/16 11:40:17 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_infile(t_cmd *cmd, t_minishell *mshell, int i)
{
	struct stat	buf;
	int			access_status;

	access_status = access(cmd->infile[i], F_OK);
	if (access_status == -1)
	{
		mshell->invalid_redir = cmd->infile[i];
		return (perror(cmd->infile[i]), 1);
	}
	if (stat(cmd->infile[i], &buf) == -1)
	{
		mshell->invalid_redir = cmd->infile[i];
		return (perror(cmd->infile[i]), 1);
	}
	if (is_directory(&buf, cmd, mshell, i) == 1)
		return (1);
	if (access(cmd->infile[i], R_OK) == -1)
	{
		mshell->invalid_redir = cmd->infile[i];
		return (perror(cmd->infile[i]), 1);
	}
	return (0);
}

int	process_outfile(t_cmd *cmd, t_minishell *mshell, int i)
{
	struct stat	buf;
	char		*tmp;
	int			access_status;

	tmp = NULL;
	access_status = access(cmd->outfile[i], F_OK);
	if (access_status == -1)
		return (0);
	if (stat(cmd->outfile[i], &buf) == -1)
		return (perror(cmd->outfile[i]), 1);
	if (((buf.st_mode) & 0170000) == (0040000))
	{
		if_is_directory(cmd, mshell, i, tmp);
		return (1);
	}
	if (access(cmd->outfile[i], W_OK) == -1)
	{
		if (mshell->invalid_redir == NULL || is_redir_before(cmd, \
				&mshell->invalid_redir, &cmd->outfile[i]))
			mshell->invalid_redir = cmd->outfile[i];
		return (perror(cmd->outfile[i]), 1);
	}
	return (0);
}

int	handle_file_redirection(t_minishell *mshell, t_cmd *cmd, \
		int old[2], int new[2])
{
	t_redir_args	args;

	args.cmd = cmd;
	args.mshell = mshell;
	args.old[0] = old[0];
	args.old[1] = old[1];
	args.new[0] = new[0];
	args.new[1] = new[1];
	args.i = 0;
	if (cmd && (cmd->infile || cmd->outfile))
	{
		if (cmd->infile && cmd->infile[args.i])
		{
			if (check_infiles(cmd, mshell) == 1)
			{
				cmd->is_valid_cmd = false;
				mshell->last_exit_status = 1;
				return (1);
			}
		}
		if (handle_outfiles(&args))
			return (1);
		handle_redirections(cmd, old, new, mshell);
	}
	return (0);
}

int	handle_outfiles(t_redir_args *args)
{
	int	fd;

	if (args->cmd->outfile && args->cmd->outfile[args->i])
	{
		while (args->cmd->outfile[args->i])
		{
			if (check_outfiles(args->cmd, args->mshell, args->i) == 1)
			{
				args->mshell->last_exit_status = 1;
				return (1);
			}
			fd = open(args->cmd->outfile[args->i], O_WRONLY | O_CREAT, 0644);
			if (fd == -1)
				return (perror(args->cmd->outfile[args->i]), 1);
			close(fd);
			args->i++;
		}
	}
	return (0);
}

void	handle_redirections(t_cmd *cmd, int old[2], int new[2], \
		t_minishell *mshell)
{
	if (cmd->op_type[0] == RED_IN || cmd->op_type[0] == HDOC)
		handle_red_in(cmd, mshell);
	if (cmd->op_type[1] == RED_OUT)
		if (handle_red_out(cmd, mshell))
			return ;
	if (cmd->op_type[1] == APP_OUT)
		handle_append_out(cmd, old, new, mshell);
}
