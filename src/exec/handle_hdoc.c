/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_hdoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:06:46 by scrumier          #+#    #+#             */
/*   Updated: 2024/07/22 16:46:53 by scrumier         ###   ########.fr       */
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
	while (g_sig == 0)
	{
		signal(SIGINT, signal_here_doc);
		rl_event_hook = event;
		line = readline("> ");
		if (!line)
			if (g_sig == 0)
				break ;
		if (g_sig != 0)
		{
			ft_printf("\n");
			break ;
		}
		if (ft_strncmp(line, cmd->infile[i], \
				ft_strlen(cmd->infile[i]) + 1) == 0)
			break ;
		ft_putendl_fd(line, fd);
		free_null(line);
	}
	if (line)
		free_null(line);
	g_sig = 0;
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
			return (free_tab(tmp_filename), 1);
		free_null(cmd->infile[i]);
		cmd->infile[i] = ft_strdup(tmp_filename[i]);
		close(fd);
		i++;
	}
	if (tmp_filename)
		free_tab(tmp_filename);
	cmd->op_type[0] = RED_IN;
	return (0);
}

void	generate_unique_filenames(t_cmd *cmd, char ***tmp_filename, \
		int old[2], int new[2])
{
	size_t	filename_length;
	int		i;
	t_cmd	*tmp;

	tmp = cmd;
	filename_length = ft_strlen(TMP_FILE_PREFIX) + (RANDOM_BYTES * 2) + 1;
	*tmp_filename = ft_calloc(ft_tablen(tmp->infile) + 1, sizeof(char *));
	if (!(*tmp_filename))
		error_pipe("malloc failed", new, old, tmp);
	i = 0;
	while ((*tmp_filename) && i < ft_tablen(tmp->infile))
	{
		(*tmp_filename)[i] = (char *)ft_calloc(filename_length, sizeof(char));
		if (!(*tmp_filename)[i])
			error_pipe("malloc failed", new, old, tmp);
		generate_unique_filename((*tmp_filename)[i], filename_length);
		i++;
	}
	if (DEBUG && (*tmp_filename))
		print_tab(*tmp_filename);
}

int	handle_heredoc_operations(t_cmd *cmd, int old[2], int new[2], \
		char **tmp_filename)
{
	t_cmd	*tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->op_type[0] == HDOC)
		{
			if (handle_hdoc(tmp, old, new, tmp_filename) == 1)
				return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	replace_hdoc(t_cmd *cmd, int old[2], int new[2])
{
	char	**tmp_filename;
	t_cmd	*tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->op_type[0] == HDOC)
			generate_unique_filenames(cmd, &tmp_filename, old, new);
		tmp = tmp->next;
	}
	if (handle_heredoc_operations(cmd, old, new, tmp_filename))
		return (1);
	return (0);
}
