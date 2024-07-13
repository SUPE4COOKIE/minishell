/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 22:16:55 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/07/13 13:07:28 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_mshell(t_minishell *mshell)
{
	free_env_path(mshell);
	free_cmds(mshell->cmds);
	free_null(mshell->line);
}