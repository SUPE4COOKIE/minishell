/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 13:11:12 by scrumier          #+#    #+#             */
/*   Updated: 2024/05/15 15:58:02 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_pipe(char *msg, int new[2], int old[2], t_cmd *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	if (old[0])
		close(old[0]);
	if (old[1])
		close(old[1]);
	if (new[0])
		close(new[0]);
	if (new[1])
		close(new[1]);
	if (cmd)
		free(cmd);
	exit(1);
}