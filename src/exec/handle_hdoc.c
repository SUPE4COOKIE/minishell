/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_hdoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:06:46 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/16 11:37:58 by scrumier         ###   ########.fr       */
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
int	read_the_line(int fd, char **hdoc)
{
	char	*line;

	line = NULL;
	g_sig = 0;
	while (42)
	{
		signal(SIGINT, signal_here_doc);
		rl_event_hook = event;
		line = readline("> ");
		if (!line)
			if (g_sig == 0)
				return (1);
		if (g_sig != 0)
		{
			ft_printf("\n");
			break ;
		}
		if (ft_strncmp(line, *hdoc, \
				ft_strlen(*hdoc) + 1) == 0)
			break ;
		ft_putendl_fd(line, fd);
		free(line);
	}
	free(line);
	return (0);
}

int	handle_hdoc(t_cmd *cmd, char **tmp_filename)
{
	int		fd;
	size_t	i;
	size_t	j;

	i = 0;
	while (cmd)
	{
		j = 0;
		while (tmp_filename && tmp_filename[i] && cmd->hdoc && cmd->hdoc[j])
		{
			fd = open(tmp_filename[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				return (perror("open"), 1);
			read_the_line(fd, cmd->hdoc[j]);
			free(*(cmd->hdoc[j]));
			*(cmd->hdoc[j]) = ft_strdup(tmp_filename[i]);
			close(fd);
			j++;
			i++;
		}
		cmd = cmd->next;
	}
	return (0);
}

int	generate_unique_filenames(t_cmd *cmd, char ***tmp_filename, size_t *k)
{
	size_t	filename_length;
	size_t	i;
	t_cmd	*tmp;

	tmp = cmd;
	filename_length = ft_strlen(TMP_FILE_PREFIX) + (RANDOM_BYTES + 1);
	if ((*tmp_filename) == NULL)
		return (1);
	i = 0;
	while ((*tmp_filename) && i < (size_t)ft_tablen(*tmp->hdoc))
	{
		(*tmp_filename)[*k] = generate_unique_filename((*tmp_filename)[*k], \
								filename_length);
		(*k)++;
		i++;
	}
	return (0);
}

size_t	count_hdoc(t_cmd *cmd)
{
	t_cmd	*tmp;
	size_t	i;
	size_t	count;

	tmp = cmd;
	count = 0;
	while (tmp)
	{
		i = 0;
		while (tmp->hdoc[i])
			i++;
		count += i;
		tmp = tmp->next;
	}
	return (count);
}

int	replace_hdoc(t_cmd *cmd)
{
	char	**tmp_filename;
	t_cmd	*tmp;
	size_t	k;

	tmp = cmd;
	k = 0;
	tmp_filename = ft_calloc(count_hdoc(cmd) + 1, sizeof(char **));
	while (tmp)
	{
		if (generate_unique_filenames(tmp, &tmp_filename, &k))
			return (1);
		tmp = tmp->next;
	}
	handle_hdoc(cmd, tmp_filename);
	free_tab(tmp_filename);
	return (0);
}
