/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_delete_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 16:42:45 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/04 17:08:04 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->cmd)
		free(cmd->cmd);
	if (cmd->args)
		free_str_array(cmd->args);
	if (cmd->op_type[0] != UNDEFINED && cmd->infile)
		free_str_array(cmd->infile);
	if (cmd->op_type[1] != UNDEFINED && cmd->outfile)
		free_str_array(cmd->outfile);
	if (cmd->type_chain)
		free(cmd->type_chain);
	free(cmd);
}

void	free_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free_cmd(cmd);
		cmd = tmp;
	}
}

t_cmd	*delete_cmd(t_cmd **cmd, t_cmd *to_delete)
{
	t_cmd	*tmp;
	t_cmd	*next;
	
	tmp = *cmd;
	if (*cmd == to_delete)
	{
		*cmd = to_delete->next;
		if (*cmd)
			(*cmd)->prev = NULL;
		free_cmd(to_delete);
		return (*cmd);
	}
	while (tmp && tmp != to_delete)
		tmp = tmp->next;
	if (tmp && tmp == to_delete)
	{
		if (tmp->prev)
			tmp->prev->next = tmp->next;
		if (tmp->next)
			tmp->next->prev = tmp->prev;
		next = tmp->next;
		free_cmd(tmp);
		return (next);
	}
	return (NULL);
}