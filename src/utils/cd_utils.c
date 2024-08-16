/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:07:29 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/16 13:36:46 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_slash_cd(char *path)
{
	path[0] = '/';
	path[1] = '\0';
}

void	set_sig_and_exit_status(t_minishell *mshell)
{
	if (g_sig == SIGINT)
	{
		mshell->last_exit_status = 130;
		rl_done = 1;
	}
}

int	is_directory(struct stat *buf, t_cmd *cmd, t_minishell *mshell, int i)
{
	char	*tmp;

	tmp = NULL;
	if (((buf->st_mode) & 0170000) == (0040000))
	{
		mshell->invalid_redir = cmd->infile[i];
		tmp = ft_strjoin(cmd->infile[i], ": is a directory\n");
		if (!*tmp)
			return (1);
		write(2, tmp, ft_strlen(tmp));
		free_null(tmp);
		return (1);
	}
	return (0);
}
