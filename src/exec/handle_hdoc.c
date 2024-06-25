/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_hdoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:06:46 by scrumier          #+#    #+#             */
/*   Updated: 2024/06/14 14:39:52 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern bool g_in_hdoc;

/**
 * @brief Read the line
 * @param line The line
 * @param fd The file descriptor
 * @param cmd The command
 * @param i The index
 */
void	read_the_line(char *line, int fd, t_cmd *cmd, int i)
{
	while (42)
	{
		g_in_hdoc = true;
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = readline("");
		g_in_hdoc = false;
		printf("line = %s\n", line);
		printf("cmd->infile[i] = %s\n", cmd->infile[i]);
		if (ft_strncmp(line, cmd->infile[i], ft_strlen(cmd->infile[i])) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}

/**
 * @brief Handle here document
 * @param cmd The command
 * @param old The old file descriptors
 * @param new The new file descriptors
 * @param tmp_filename The temporary filename
 */
void	handle_hdoc(t_cmd *cmd, int old[2], int new[2], char *tmp_filename)
{
	int		fd;
	char	*line;
	int		i;

	line = NULL;
	fd = open(tmp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		error_pipe("open failed", new, old, cmd);
	i = 0;
	while (cmd->infile[i])
	{
		read_the_line(line, fd, cmd, i);
		i++;
	}
	if (dup2(fd, STDIN_FILENO) == -1)
		error_pipe("dup2 failed", new, old, cmd);
	close(fd);
	cmd->infile[0] = ft_strdup(tmp_filename);
	free(tmp_filename);
	cmd->op_type[0] = RED_IN;
}

/**
 * @brief Replace the here document
 * @param cmd The command
 * @param old The old file descriptors
 * @param new The new file descriptors
 */
void	replace_hdoc(t_cmd *cmd, int old[2], int new[2])
{
	int		i;
	char	*tmp_filename;
	size_t	filename_length;

	i = 0;
	filename_length = strlen(TMP_FILE_PREFIX) + (RANDOM_BYTES * 2) + 1;
	tmp_filename = (char *)malloc(sizeof(char) * filename_length);
	if (!tmp_filename)
		return ;
	generate_unique_filename(tmp_filename, filename_length);
	if (DEBUG)
		printf("filename -> %s\n", tmp_filename);
	while (cmd)
	{
		if (cmd->op_type[0] == HDOC)
			handle_hdoc(cmd, old, new, tmp_filename);
		cmd = cmd->next;
		i++;
	}
}
