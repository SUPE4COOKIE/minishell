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
		line = readline("> ");
		if (DEBUG)
		{
			printf("line = %s\n", line);
			printf("cmd->infile[i] = %s\n", cmd->infile[i]);
		}
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
void	handle_hdoc(t_cmd *cmd, int old[2], int new[2], char **tmp_filename)
{
	int		fd;
	char	*line;
	int		i;

	line = NULL;
	i = 0;
	while (tmp_filename && tmp_filename[i] && cmd->infile && cmd->infile[i])
	{
		fd = open(tmp_filename[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			error_pipe("open failed", new, old, cmd);
		read_the_line(line, fd, cmd, i);
		free(cmd->infile[i]);
		cmd->infile[i] = ft_strdup(tmp_filename[i]);
		close(fd);
		i++;
	}
	if (tmp_filename)
		free_tab(tmp_filename);
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
		char	**tmp_filename;
		size_t	filename_length;
		int		i;

		while (cmd && cmd->cmd) {
			filename_length = strlen(TMP_FILE_PREFIX) + (RANDOM_BYTES * 2) + 1;
			tmp_filename = (char **)malloc(sizeof(char **) * ft_tablen(cmd->infile) + 1);
			if (!tmp_filename)
				error_pipe("malloc failed", new, old, cmd);
			i = 0;
			while (tmp_filename && tmp_filename[i] && i < ft_tablen(cmd->infile))
			{
				tmp_filename[i] = (char *)malloc(sizeof(char *) * filename_length + 1);
				if (!tmp_filename[i])
					error_pipe("malloc failed", new, old, cmd);
				generate_unique_filename(tmp_filename[i], filename_length);
				i++;
			}
			if (DEBUG)
				print_tab(tmp_filename);

		}
		while (cmd)
		{
			if (cmd->op_type[0] == HDOC)
				handle_hdoc(cmd, old, new, tmp_filename);
			cmd = cmd->next;
		}
	}
