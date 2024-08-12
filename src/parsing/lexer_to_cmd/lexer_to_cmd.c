/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_to_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 17:23:05 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/10 15:19:03 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_last_cmd(t_cmd *last_cmd, char **args_start)
{
	last_cmd->args = args_start;
	if (*args_start)
	{
		last_cmd->cmd = ft_strdup(args_start[0]);
		if (!last_cmd->cmd)
			return (-1);
	}
	else
	{
		last_cmd->cmd = NULL;
		free_tab(args_start);
		last_cmd->args = NULL;
		last_cmd->is_valid_cmd = true;
	}
	return (0);
}

int	append_cmds(t_cmd **cmd, t_lexer **lex)
{
	char	**args;
	char	**args_start;
	t_cmd	*last_cmd;

	if (allocate_args_cmd(*lex, &args, &args_start, &last_cmd) == -1)
		return (-1);
	append_cmd(cmd, last_cmd);
	while (*lex)
	{
		if (is_redir((*lex)->type))
		{
			if (append_redir(last_cmd, lex) == -1)
				return (-1);
		}
		else if ((*lex)->value && is_nospace_addable((*lex)->type))
		{
			if (handle_args(&args, lex) == -1)
				return (-1);
		}
		else
			break ;
		if (*lex)
			*lex = (*lex)->next;
	}
	return (set_last_cmd(last_cmd, args_start) == -1);
}

int	resolve_cmd_path(t_cmd **cmd, char **path, int *exit_status)
{
	t_cmd	*tmp;

	tmp = *cmd;
	while (tmp)
	{
		if (!tmp->cmd && tmp->is_valid_cmd)
		{
			tmp = tmp->next;
			continue ;
		}
		if (get_cmd_path(&tmp, path, exit_status) == -1)
			return (-1);
		if (!tmp->is_valid_cmd)
		{
			free(tmp->cmd);
			tmp->cmd = NULL;
		}
		tmp = tmp->next;
	}
	return (0);
}

t_cmd	*lexer_to_cmd(t_lexer *lex, char **path, int *exit_status)
{
	t_cmd	*cmd;
	t_lexer	*current_lex;

	cmd = NULL;
	current_lex = lex;
	while (current_lex)
	{
		append_cmds(&cmd, &current_lex);
		if (current_lex)
			current_lex = current_lex->next;
	}
	resolve_cmd_path(&cmd, path, exit_status);
	return (cmd);
}
