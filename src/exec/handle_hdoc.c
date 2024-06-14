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
 * @brief Handle here document
 * @param cmd
 * @param old
 * @param new
 */

void	handle_hdoc(t_cmd *cmd, int old[2], int new[2])
{
	int		fd;
	char	*tmp_filename;
	char	*line;
	int		i;

	i = 0;
	tmp_filename = ft_strjoin("/tmp/minishell_hdoc_", ft_itoa(i));
	fd = open(tmp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		error_pipe("open failed", new, old, cmd);
	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (cmd->outfile && ft_strncmp(line, cmd->outfile[0], \
			ft_strlen(cmd->outfile[0])) == 0)
			break ;
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
		error_pipe("dup2 failed", new, old, cmd);
	close(fd);
}

void	replace_hdoc(t_cmd *cmd, int old[2], int new[2])
{
	while (cmd)
	{
		if (cmd->op_type[0] == HDOC)
			handle_hdoc(cmd, old, new);
		cmd = cmd->next;
	}
}
