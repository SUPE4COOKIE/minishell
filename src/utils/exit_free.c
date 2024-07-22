/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 22:16:55 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/07/22 16:58:50 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_mshell(t_minishell *mshell)
{
	free_env_path(mshell);
	free_cmds(mshell->cmds);
	free_null(mshell->line);
}

void	free_lst(t_arg *new_args)
{
	t_arg	*tmp;

	while (new_args)
	{
		tmp = new_args;
		new_args = new_args->next;
		free_null(tmp->arg);
		free_null(tmp);
	}
}
