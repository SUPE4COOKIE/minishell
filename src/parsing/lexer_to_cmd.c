/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_to_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 17:23:05 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/05/18 21:57:30 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_type	token_to_cmd(t_token_type type)
{
	if (type == T_WORD)
		return (CMD);
	else if (type == T_PIPE)
		return (PIP);
	else if (type == T_REDIR_IN)
		return (RED_IN);
	else if (type == T_REDIR_OUT)
		return (RED_OUT);
	else if (type == T_APPEND_OUT)
		return (APP_OUT);
	else if (type == T_HERE_DOC)
		return (HDOC);
	return (CMD);
}

t_cmd	*new_cmd(t_token_type type, char *value)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->type = token_to_cmd(type);
	new->cmd = value;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

int	append_cmd(t_cmd **cmd, t_cmd *new)
{
	t_cmd	*tmp;

	if (*cmd == NULL)
	{
		*cmd = new;
		(*cmd)->prev = NULL;
	}
	else
	{
		tmp = *cmd;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
		new->prev = tmp;
	}
	return (0);
}

size_t	get_cmd_size(t_lexer *lex)
{
	size_t	size;

	size = 0;
	while (lex)
	{
		if (lex->value && (lex->type == T_WORD || lex->type == T_S_QUOTED_WORD
			|| lex->type == T_D_QUOTED_WORD))
			size += ft_strlen(lex->value) + 1;
		lex = lex->next;
	}
	return (size);
}

int	append_words(t_cmd **cmd, t_lexer **lex)
{
	(void)cmd;
	char	*cmd_str;

	cmd_str = malloc(sizeof(char) * (get_cmd_size(*lex) + 1));
	if (!cmd_str)
		return (1);
	cmd_str[0] = 0;
	while (*lex)
	{
		if ((*lex)->value && ((*lex)->type == T_WORD || (*lex)->type == T_S_QUOTED_WORD
			|| (*lex)->type == T_D_QUOTED_WORD))
		{
			if (cmd_str[0])
				ft_strlcat(cmd_str, " ", get_cmd_size(*lex) + 1);
			ft_strlcat(cmd_str, (*lex)->value, get_cmd_size(*lex) + 1);
		}
		else
			break ;
		*lex = (*lex)->next;
	}
	printf("cmd: %s\n", cmd_str);
	return (0);
}

t_cmd	*lexer_to_cmd(t_lexer *lex)
{
	t_cmd	*cmd;
	t_lexer	*current_lex;

	cmd = NULL;
	current_lex = lex;
	while (current_lex)
	{
		if (current_lex->type == T_WORD || current_lex->type == T_S_QUOTED_WORD
			|| current_lex->type == T_D_QUOTED_WORD)
			append_words(&cmd, &current_lex);
		if (current_lex)
			current_lex = current_lex->next;
	}
	return (cmd);
}