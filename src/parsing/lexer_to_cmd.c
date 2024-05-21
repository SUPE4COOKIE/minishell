/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_to_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 17:23:05 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/05/21 23:22:57 by mwojtasi         ###   ########.fr       */
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

char	*cmd_type_to_str(t_cmd_type type)
{
	if (type == CMD)
		return ("CMD");
	else if (type == PIP)
		return ("PIP");
	else if (type == RED_IN)
		return ("RED_IN");
	else if (type == RED_OUT)
		return ("RED_OUT");
	else if (type == APP_OUT)
		return ("APP_OUT");
	else if (type == HDOC)
		return ("HDOC");
	return ("CMD");
}

t_cmd	*new_cmd(t_cmd_type type, char **args)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->op_type = type;
	new->args = args;
	new->cmd = args[0];
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

t_cmd	*get_last_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;

	tmp = cmd;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
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

size_t	get_word_num(t_lexer *lex)
{
	size_t	num;

	num = 0;
	while (lex)
	{
		if (lex->value && (lex->type == T_WORD || lex->type == T_S_QUOTED_WORD
			|| lex->type == T_D_QUOTED_WORD))
			num++;
		lex = lex->next;
	}
	return (num);
}

t_cmd_type get_op_type(t_lexer *lex)
{
	if (!lex)
		return (CMD);
	else
		return (token_to_cmd(lex->type));
}

int append_pipe(t_cmd **cmd, t_lexer **lex)
{
	get_last_cmd(*cmd)->pipe = PIP;
	*lex = (*lex)->next;
	// might rather create a function
	if ((*lex)->type == T_WORD || (*lex)->type == T_S_QUOTED_WORD
			|| (*lex)->type == T_D_QUOTED_WORD)
			append_words(cmd, lex); //check return                     
	get_last_cmd(*cmd)->op_type = PIP;
	return (0);
}

void	print_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;

	tmp = cmd;
	while (tmp)
	{
		printf("cmd: %s\n", tmp->cmd);
		printf("args: ");
		for (int i = 0; tmp->args[i]; i++)
			printf("%s ", tmp->args[i]);
		printf("\noperation: %s\n", cmd_type_to_str(tmp->op_type));
		printf("\n");
		tmp = tmp->next;
	}
}

int	append_words(t_cmd **cmd, t_lexer **lex)
{
	// need to add system to not add paths for redirections
	char	**args;
	char	**args_start;

	args = ft_calloc(get_word_num(*lex) + 1, sizeof(char *));
	args_start = args;
	if (!args)
		return (1);
	while (*lex)
	{
		if ((*lex)->value && ((*lex)->type == T_WORD || (*lex)->type == T_S_QUOTED_WORD
			|| (*lex)->type == T_D_QUOTED_WORD))
		{
			*args = ft_strdup((*lex)->value);
			args++;
		}
		else
			break ;
		*lex = (*lex)->next;
	}
	append_cmd(cmd, new_cmd(CMD, args_start));
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
		if (current_lex && current_lex->type == T_PIPE)
			append_pipe(&cmd, &current_lex);
		if (current_lex)
			current_lex = current_lex->next;
	}
	print_cmd(cmd);
	return (cmd);
}