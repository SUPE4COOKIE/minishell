/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:43:36 by scrumier          #+#    #+#             */
/*   Updated: 2024/06/05 13:26:22 by scrumier         ###   ########.fr       */
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
	if (old[0])
		close(old[0]);
	if (old[1])
		close(old[1]);
	if (new[0])
		close(new[0]);
	if (new[1])
		close(new[1]);
}

/**
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

/**
 * @brief Execute a builtin command
 * @param mshell
 * @param cmd
 */
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

/**
 * @brief Check if the command is a builtin command
 * @param cmd
 * @return true if the command is a builtin command, false otherwise
 */
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

char **copy_args(char **args)
{
    int arg_count;
    char **args_copy;
    int i;

	arg_count = 0;
	args_copy = NULL;
	i = 0;
    while (args[arg_count])
        arg_count++;
    args_copy = malloc((arg_count + 1) * sizeof(char *));
    if (args_copy == NULL) {
        return NULL;
    }

    while (args[i] != NULL) {
        args_copy[i] = strdup(args[i]);
        if (args_copy[i] == NULL) {
            // If allocation fails, free any previously allocated strings
            while (--i >= 0) {
                free(args_copy[i]);
            }
            free(args_copy);
            return NULL;
        }
        i++;
    }

    args_copy[arg_count] = NULL; // Null-terminate the array

    return args_copy;
}

/**
 * @brief Execute a command
 * @param mshell
 * @param cmd
 */
void exec_cmd(t_minishell *mshell, t_cmd *cmd)
{
	char *program;
	char **args;

	if (is_builtin(cmd->cmd) == true)
	{
		exec_builtin(mshell, cmd);
	}
	else
	{
		program = strdup(cmd->cmd);
		if (program == NULL) {
			perror("Failed to allocate memory for program path");
			return ;
		}
		args = copy_args(cmd->args);
		if (args == NULL) {
			perror("Failed to allocate memory for arguments");
			free(program);
			return ;
		}
		execve(program, args, mshell->env);
		free(program);
		free_tab(args);
	}
}

/**
 * @brief Handle file redirection
 * @param mshell
 * @param cmd
 * @param old
 * @param new
 */
void handle_file_redirection(t_minishell *mshell, t_cmd *cmd, int old[2], int new[2])
{
	int fd;
	int i;

	i = 0;
	(void)mshell;
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
		close(fd);
	}
	else if (cmd->op_type[1] == APP_OUT)
	{
		fd = open(cmd->outfile[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			error_pipe("open failed", new, old, cmd);
		if (dup2(fd, STDOUT_FILENO) == -1)
			error_pipe("dup2 failed", new, old, cmd);
		close(fd);
	}
	else if (cmd->op_type[0] == RED_IN)
	{
		fd = open(cmd->infile[0], O_RDONLY);
		if (fd == -1)
			error_pipe("open failed", new, old, cmd);
		if (dup2(fd, STDIN_FILENO) == -1)
			error_pipe("dup2 failed", new, old, cmd);
		close(fd);
	}
}

/**
 * @brief Duplicate the file descriptors
 * @param i
 * @param cmd
 * @param old
 * @param new
 */
void dup_cmd(int i, t_cmd *cmd, int old[2], int new[2])
{
	if (i != 0)
	{
		if (dup2(old[0], STDIN_FILENO) == -1)
		{
			perror("dup2 failed");
			exit(EXIT_FAILURE);
		}
		if (old[0] != -1)
			close(old[0]);
		if (old[1] != -1)
			close(old[1]);
	}
	if (cmd->next)
	{
		if (dup2(new[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 failed");
			exit(EXIT_FAILURE);
		}
		if (new[0] != -1)
			close(new[0]);
		if (new[1] != -1)
			close(new[1]);
	}
}

void replace_hdoc(t_cmd *cmd)
{
	int fd;
	char *tmp_filename;
	char *line;
	int i;

	i = 0;
	while (cmd)
	{
		if (cmd->op_type[0] == HDOC)
		{
			tmp_filename = tmp_file(i++);
			fd = open(tmp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
			if (fd == -1)
			{
				perror("open");
				exit(EXIT_FAILURE);
			}

			while (1)
			{
				line = readline("> ");
				if (!line || ft_strncmp(line, cmd->infile[0], ft_strlen(cmd->infile[0])) == 0)
				{
					free(line);
					break;
				}
				write(fd, line, ft_strlen(line));
				write(fd, "\n", 1);
				free(line);
			}
			close(fd);

			// Replace heredoc with input redirection
			free(cmd->infile[0]);
			cmd->infile[0] = ft_strdup(tmp_filename);
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
	int i;

	init_old_new(old, new);
	cmd = mshell->cmds;
	replace_hdoc(cmd);
	cmd = mshell->cmds;
	i = 0;
	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(new) == -1)
				error_pipe("pipe failed", new, old, cmd);
		}
		if (is_builtin(cmd->cmd) == false)
			id = fork();
		else if (is_builtin(cmd->cmd) == true && cmd->next)
			id = fork();
		else
			id = 0;
		if (id == -1)
			error_pipe("fork failed", new, old, cmd);
		if (id == 0)
		{
			dup_cmd(i, cmd, old, new);
			handle_file_redirection(mshell, cmd, old, new);
			exec_cmd(mshell, cmd);
			if (is_builtin(cmd->cmd) == false)
				exit(EXIT_FAILURE);
		}
		else
		{
			if (i != 0)
			{
				if (old[0] != -1)
					close(old[0]);
				if (old[1] != -1)
					close(old[1]);
			}
			old[0] = new[0];
			old[1] = new[1];
		}
		i++;
		cmd = cmd->next;
	}
	ft_close(old, new);
	while (waitpid(-1, NULL, 0) != -1)
		;
}
