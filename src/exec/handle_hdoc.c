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
int	read_the_line(char *line, int fd, t_cmd *cmd, int i)
{
	int initial_sig;

	initial_sig = g_sig;
	signal(SIGINT, signal_here_doc);
	while (g_sig == initial_sig)
	{
		line = readline("> ");
		if (!line)
		{
			if (g_sig != initial_sig)
				break ;
			continue ;
		}
		if (g_sig != initial_sig)
		{
			free(line);
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
			g_sig = initial_sig;
			return (1);
		}
		if (DEBUG)
		{
			printf("line = %s\n", line);
			printf("cmd->infile[i] = %s\n", cmd->infile[i]);
		}
		if (ft_strncmp(line, cmd->infile[i], ft_strlen(cmd->infile[i]) + 1) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	signal(SIGINT, signal_new_line);
	g_sig = initial_sig;
	return (0);
}

/**
 * @brief Handle here document
 * @param cmd The command
 * @param old The old file descriptors
 * @param new The new file descriptors
 * @param tmp_filename The temporary filename
 */
int	handle_hdoc(t_cmd *cmd, int old[2], int new[2], char **tmp_filename)
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
		if (read_the_line(line, fd, cmd, i) == 1)
			return (1);
		free(cmd->infile[i]);
		cmd->infile[i] = ft_strdup(tmp_filename[i]);
		close(fd);
		i++;
	}
	if (tmp_filename)
		free_tab(tmp_filename);
	cmd->op_type[0] = RED_IN;
	return (0);
}

/**
 * @brief Replace the here document
 * @param cmd The command
 * @param old The old file descriptors
 * @param new The new file descriptors
 */
int	replace_hdoc(t_cmd *cmd, int old[2], int new[2])
{
	char	**tmp_filename;
	size_t	filename_length;
	int		i;
	t_cmd	*tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->op_type[0] != HDOC)
		{
			tmp = tmp->next;
			continue ;
		}
		filename_length = strlen(TMP_FILE_PREFIX) + (RANDOM_BYTES * 2) + 1;
		tmp_filename = (char **)ft_calloc(ft_tablen(tmp->infile) + 1, sizeof(char *));
		if (!tmp_filename)
			error_pipe("malloc failed", new, old, tmp);
		i = 0;
		while (tmp_filename && i < ft_tablen(tmp->infile))
		{
			tmp_filename[i] = (char *)ft_calloc(filename_length, sizeof(char));
			if (!tmp_filename[i])
				error_pipe("malloc failed", new, old, tmp);
			generate_unique_filename(tmp_filename[i], filename_length);
			i++;
		}
		if (DEBUG && tmp_filename)
			print_tab(tmp_filename);
		tmp = tmp->next;
	}
	tmp = cmd;
	while (tmp)
	{
		if (tmp->op_type[0] == HDOC)
			if (handle_hdoc(tmp, old, new, tmp_filename) == 1)
				return (1);
		tmp = tmp->next;
	}
	return (0);
}
