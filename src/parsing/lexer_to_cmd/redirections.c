/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 16:53:44 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/04 17:00:34 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_redir_in(t_cmd *cmd, t_lexer **lex)
{
	(*lex) = (*lex)->next;
	cmd->op_type[0] = RED_IN;
	if (ft_append_str(&(cmd->infile), (*lex)->value) == -1)
		return (-1);
	return (append_type(&(cmd->type_chain), RED_IN));
}

int handle_redir_out(t_cmd *cmd, t_lexer **lex, int type)
{
	(*lex) = (*lex)->next;
	cmd->op_type[1] = type;
	if (ft_append_str(&(cmd->outfile), (*lex)->value) == -1)
		return (-1);
	return (append_type(&(cmd->type_chain), type));
}

int handle_here_doc(t_cmd *cmd, t_lexer **lex)
{
	(*lex) = (*lex)->next;
	cmd->op_type[0] = HDOC;
	if (ft_append_str(&(cmd->infile), (*lex)->value) == -1)
		return (-1);
	return (append_type(&(cmd->type_chain), HDOC));
}

int append_redir(t_cmd *cmd, t_lexer **lex)
{
	if ((*lex)->type == T_REDIR_IN)
		return (handle_redir_in(cmd, lex));
	else if ((*lex)->type == T_REDIR_OUT)
		return (handle_redir_out(cmd, lex, RED_OUT));
	else if ((*lex)->type == T_APPEND_OUT)
		return (handle_redir_out(cmd, lex, APP_OUT));
	else if ((*lex)->type == T_HERE_DOC)
		return (handle_here_doc(cmd, lex));
	return (0);
}

bool is_redir(t_token_type type)
{
	return (type == T_REDIR_IN || type == T_REDIR_OUT
		|| type == T_APPEND_OUT || type == T_HERE_DOC);
}