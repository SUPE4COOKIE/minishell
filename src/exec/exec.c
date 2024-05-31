/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 13:05:02 by sonamcrumie       #+#    #+#             */
/*   Updated: 2024/05/31 16:38:58 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** @brief Close the file descriptors
** @param old The old file descriptors
** @param new The new file descriptors
*/
void	ft_close(int old[2], int new[2])
{
	if (old[0])
		close(old[0]);
	if (old[1])
		close(old[1]);
	if (new[0])
		close(new[0]);
	if (new[1])
		close(new[1]);
}

/*
** @brief Initialize the old and new file descriptors
** @param old The old file descriptors
** @param new The new file descriptors
*/
static void	init_old_new(int old[2], int new[2])
{
	old[0] = -1;
	old[1] = -1;
	new[0] = -1;
	new[1] = -1;
}

void	exec_builtin(t_minishell *mshell, t_cmd *cmd)
{
	if (ft_strncmp(cmd->cmd, "echo", ft_strlen(cmd->cmd)) == 0)
		builtin_echo(mshell, cmd->args);
	if (ft_strncmp(cmd->cmd, "cd", ft_strlen(cmd->cmd)) == 0)
		builtin_cd(mshell, cmd->args);
	if (ft_strncmp(cmd->cmd, "pwd", ft_strlen(cmd->cmd)) == 0)
		builtin_pwd(mshell);
	if (ft_strncmp(cmd->cmd, "export", ft_strlen(cmd->cmd)) == 0)
		//builtin_export // TODO : implement export
	if (ft_strncmp(cmd->cmd, "unset", ft_strlen(cmd->cmd)) == 0)
		builtin_unset(mshell, cmd->args);
	if (ft_strncmp(cmd->cmd, "env", ft_strlen(cmd->cmd)) == 0)
		builtin_env(mshell, cmd->args);
	if (ft_strncmp(cmd->cmd, "exit", ft_strlen(cmd->cmd)) == 0)
		builtin_exit(mshell, cmd->args);
}

bool is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", ft_strlen(cmd)) == 0)
		return (true);
	if (ft_strncmp(cmd, "cd", ft_strlen(cmd)) == 0)
		return (true);
	if (ft_strncmp(cmd, "pwd", ft_strlen(cmd)) == 0)
		return (true);
	if (ft_strncmp(cmd, "export", ft_strlen(cmd)) == 0)
		return (true);
	if (ft_strncmp(cmd, "unset", ft_strlen(cmd)) == 0)
		return (true);
	if (ft_strncmp(cmd, "env", ft_strlen(cmd)) == 0)
		return (true);
	if (ft_strncmp(cmd, "exit", ft_strlen(cmd)) == 0)
		return (true);
	return (false);
}

void exec_cmd(t_minishell *mshell, t_cmd *cmd)
{
	if (is_builtin(cmd->cmd) == true)
	{
		exec_builtin(mshell, cmd);
	}
	else
	{
		execve(cmd->cmd, cmd->args, mshell->env);
	}
}

void handle_file_redirection(t_minishell *mshell, t_cmd *cmd, int old[2], int new[2])
{
	int fd;
	int i;
	ssize_t n;
	pid_t pid;
	char buf[4096];
	int pipe_fd[2];

	i = 0;
	//printf("cmd->op_type[1] = %d\n", cmd->op_type[1]);
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
	{
		fd = open(cmd->outfile[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			error_pipe("open failed", new, old, cmd);
		if (dup2(fd, STDOUT_FILENO) == -1)
			error_pipe("dup2 failed", new, old, cmd);
		ft_close(old, new);
		exec_cmd(mshell, cmd);
		close(fd);
	}
	else if (cmd->op_type[1] == APP_OUT)
	{
		fd = open(cmd->outfile[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			error_pipe("open failed", new, old, cmd);
		if (dup2(fd, STDOUT_FILENO) == -1)
			error_pipe("dup2 failed", new, old, cmd);
		ft_close(old, new);
		exec_cmd(mshell, cmd);
		close(fd);
	}
	else if (cmd->op_type[0] == RED_IN)
	{
		if (pipe(new) == -1)
			error_pipe("pipe failed", new, old, cmd);

		pid = fork();
		if (pid == -1)
			error_pipe("fork failed", new, old, cmd);

		if (pid == 0)
		{
			close(new[1]);

			if (dup2(new[0], STDIN_FILENO) == -1)
				error_pipe("dup2 failed", new, old, cmd);
			close(pipe_fd[0]);

			exec_cmd(mshell, cmd);
		}
		else
		{
			close(new[0]);
			i = 0;
			while (cmd->infile[i])
			{
				fd = open(cmd->infile[i], O_RDONLY);
				if (fd == -1)
					error_pipe("open failed", new, old, cmd);
				n = 0;
				while ((n = read(fd, buf, sizeof(buf))) > 0)
				{
					if (write(new[1], buf, n) != n)
						error_pipe("write failed", new, old, cmd);
				}
				if (n == -1)
					error_pipe("read failed", new, old, cmd);
				close(fd);
				i++;
			}
			close(new[1]);
			wait(NULL);
		}
	}
}

void dup_and_exec(t_minishell *mshell, t_cmd *cmd, int old[2], int new[2])
{
	int fd[2];

	if (old[0] != -1 && old[1] != -1)
		fd[0] = old[0];
	else
		fd[0] = STDIN_FILENO;
	if (cmd->next)
		fd[1] = new[1];
	else
		fd[1] = STDOUT_FILENO;
	if (cmd->infile || cmd->outfile)
		handle_file_redirection(mshell, cmd, old, new);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		error_pipe("dup2 failedd", new, old, cmd);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		error_pipe("dup2 failed", new, old, cmd);
	ft_close(old, new);
	exec_cmd(mshell, cmd);
}

void replace_hdoc(int new[2], int old[2], t_cmd *cmd)
{
	pid_t pid;
	char *line;
	int pipe_fd[2];

	while (cmd)
	{
		if (cmd->op_type[0] == HDOC)
		{
			if (pipe(pipe_fd) == -1)
				error_pipe("pipe failed", new, old, cmd);
			pid = fork();
			if (pid == -1)
				error_pipe("fork failed", new, old, cmd);
			if (pid == 0)
			{
				close(pipe_fd[0]);
				while (42)
				{
					ft_putstr_fd("heredoc> ", STDOUT_FILENO);
					line = readline(NULL);
					if (!line)
						error_pipe("readline failed", new, old, cmd);
					if (ft_strncmp(line, cmd->infile[0], ft_strlen(cmd->infile[0])) == 0)
					{
						free(line);
						break;
					}
					ft_putendl_fd(line, new[1]);
					free(line);
				}
				close(new[1]);
				exit(EXIT_SUCCESS);
			}
			else
			{
				close(new[1]);
				dup2(new[0], STDIN_FILENO);
				close(new[0]);
			}
			cmd->op_type[0] = RED_IN;
		}
		cmd = cmd->next;
	}
}

/*
** @brief execute the commands
** @param mshell The minishell structure
*/
void	exec(t_minishell *mshell)
{
	t_cmd	*cmd;
	int		id;
	int		old[2];
	int		new[2];
	int		child_count;
	pid_t	*child_pids;
	int i;

	child_count = 1;
	init_old_new(old, new);
	cmd = mshell->cmds;
	replace_hdoc(new, old, cmd);
	while (cmd)
	{
		child_count++;
		cmd = cmd->next;
	}
	child_pids = malloc(sizeof(int) * child_count);
	if (child_pids == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	cmd = mshell->cmds;
	i = 0;
	while (cmd)
	{
		if (cmd->next)
			if (pipe(new) == -1)
				error_pipe("pipe failed", new, old, cmd);
		id = fork();
		if (id == -1)
			error_pipe("fork failed", new, old, cmd);
		if (id == 0)
		{
			dup_and_exec(mshell, cmd, old, new);
		}
		else
		{
			child_pids[i] = id;
			i++;
		}
		old[0] = new[0];
		old[1] = new[1];
		cmd = cmd->next;
	}
	ft_close(old, new);
	i = 0;
	if (waitpid(child_pids[i], NULL, 0))
		;

	free(child_pids);
}
