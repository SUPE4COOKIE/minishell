/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 11:27:28 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/12 11:28:47 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	if_is_directory(t_cmd *cmd, t_minishell *mshell, int i, char *tmp)
{
	tmp = ft_strjoin(cmd->outfile[i], ": is a directory\n");
	if (!tmp)
	{
		free_shell(mshell, 1);
		return ;
	}
	write(2, tmp, ft_strlen(tmp));
	if (mshell->invalid_redir == NULL || is_redir_before(cmd, \
			&mshell->invalid_redir, &cmd->outfile[i]))
		mshell->invalid_redir = cmd->outfile[i];
	free(tmp);
}
