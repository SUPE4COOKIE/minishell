/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_hdoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:06:46 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/14 11:12:25 by scrumier         ###   ########.fr       */
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
int	read_the_line(char *line, int fd, char **hdoc)
{
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
		free_null(line);
	}
	if (line)
		free_null(line);
	return (0);
}

int	handle_hdoc(t_cmd *cmd, char ***hdoc, char **tmp_filename)
{
	int		fd;
	char	*line;
	int		i;

	(void) cmd;
	line = NULL;
	i = 0;
	while (tmp_filename && tmp_filename[i] && hdoc && hdoc[i])
	{
		fd = open(tmp_filename[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (perror("open"), 1);
		read_the_line(line, fd, hdoc[i]);
		free_null(*(hdoc[i]));
		*(hdoc[i]) = ft_strdup(tmp_filename[i]);
		close(fd);
		i++;
	}
	if (tmp_filename)
		free_tab(tmp_filename);
	return (0);
}

int	generate_unique_filenames(t_cmd *cmd, char ***tmp_filename)
{
	size_t	filename_length;
	int		i;
	t_cmd	*tmp;

	tmp = cmd;
	filename_length = ft_strlen(TMP_FILE_PREFIX) + (RANDOM_BYTES * 2) + 1;
	*tmp_filename = ft_calloc(ft_tablen(tmp->infile) + 1, sizeof(char *));
	if (!(*tmp_filename))
		return (1);
	i = 0;
	while ((*tmp_filename) && i < ft_tablen(tmp->infile))
	{
		(*tmp_filename)[i] = (char *)ft_calloc(filename_length, sizeof(char));
		if (!(*tmp_filename)[i])
			return (1);
		generate_unique_filename((*tmp_filename)[i], filename_length);
		i++;
	}
	return (0);
}

//print a tab
void	print_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
	{
		printf("tab[%d] = %s\n", i, tab[i]);
		i++;
	}
	printf("\n");
}

char	***save_heredocs(t_cmd *cmd, size_t *k)
{
	size_t	i;
	size_t	j;
	char	***hdoc;

	i = 0;
	j = 0;
	hdoc = ft_calloc(ft_tablen(cmd->infile) + 1, sizeof(char *));
	if (!hdoc)
		return (NULL);
	while (cmd->type_chain && cmd->type_chain[j] != UNDEFINED)
	{
		if (cmd->type_chain[j] == HDOC || cmd->type_chain[j] == RED_IN)
		{
			if (cmd->type_chain[j] == HDOC)
			{
				hdoc[(*k)++] = &(cmd->infile[i]);
			}
			i++;
		}
		j++;
	}
	return (hdoc);
}

int	replace_hdoc(t_cmd *cmd)
{
	char	**tmp_filename;
	char	***hdoc;
	t_cmd	*tmp;
	size_t	k;

	tmp = cmd;
	hdoc = NULL;
	k = 0;
	while (tmp)
	{
		hdoc = save_heredocs(tmp, &k);
		if (!hdoc)
			return (1);
		if (generate_unique_filenames(tmp, &tmp_filename))
			return (1);
		tmp = tmp->next;
	}
	handle_hdoc(cmd, hdoc, tmp_filename);
	// faire un fonction pour remplacer les optype de chaque cmd par redir in
	free_null(hdoc);
	while (cmd)
	{
		cmd->op_type[0] = RED_IN;
		cmd = cmd->next;
	}
	return (0);
}
