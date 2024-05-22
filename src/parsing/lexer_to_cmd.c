/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_to_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 17:23:05 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/05/22 23:08:03 by mwojtasi         ###   ########.fr       */
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
	new->infile = NULL;
	new->outfile = NULL;
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
	printf("cmd: %s\n", new->cmd);
	printf("args: ");
	for (int i = 0; new->args[i]; i++)
		printf("%s ", new->args[i]);
	printf("\noperation: %s\n", cmd_type_to_str(new->op_type));
	printf("\n");
	return (0);
}

t_cmd	*get_last_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;

	tmp = cmd;
	if (!tmp)
		return (NULL);
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
	get_last_cmd(*cmd)->op_type = PIP;
	printf("type change: %s -> %s\n", get_last_cmd(*cmd)->cmd, cmd_type_to_str((*cmd)->op_type));
	*lex = (*lex)->next;
	// might rather create a function
	if ((*lex)->type == T_WORD || (*lex)->type == T_S_QUOTED_WORD
			|| (*lex)->type == T_D_QUOTED_WORD)
			append_words(cmd, lex); //check return 
	get_last_cmd(*cmd)->op_type = PIP;
	printf("type change: %s -> %s\n", get_last_cmd(*cmd)->cmd, cmd_type_to_str(get_last_cmd(*cmd)->op_type));
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

bool	is_valid_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->op_type == PIP && !tmp->next)
			return (false);
		tmp = tmp->next;
	}
	return (true);
}

void	delete_cmd(t_cmd **cmd)
{
	t_cmd	*tmp;
	t_cmd	*next;

	tmp = *cmd;
	while (tmp)
	{
		next = tmp->next;
		//TODO: free function
		free(tmp->cmd);
		free(tmp->args);
		free(tmp);
		tmp = next;
	}
	*cmd = NULL;
}

int	append_redir(t_cmd **cmd, t_lexer **lex)
{
	if ((*lex)->type == T_REDIR_IN)
	{
		get_last_cmd(*cmd)->op_type = RED_IN;
		printf("type change: %s -> %s\n", get_last_cmd(*cmd)->cmd, cmd_type_to_str(get_last_cmd(*cmd)->op_type));
		
	}
	return (0);
}

t_cmd	*lexer_to_cmd(t_lexer *lex, char **path)
{
	t_cmd	*cmd;
	t_lexer	*current_lex;

	(void)path;
	cmd = NULL;
	current_lex = lex;
	while (current_lex)
	{
		if (current_lex->type == T_WORD || current_lex->type == T_S_QUOTED_WORD
			|| current_lex->type == T_D_QUOTED_WORD)
			append_words(&cmd, &current_lex);
		if (current_lex && current_lex->type == T_PIPE)
			append_pipe(&cmd, &current_lex);
		if (current_lex && (current_lex->type == T_REDIR_IN || current_lex->type == T_REDIR_OUT
			|| current_lex->type == T_APPEND_OUT))
			append_redir(&cmd, &current_lex);
		if (current_lex)
			current_lex = current_lex->next;
	}
	//print_cmd(cmd);
	return (cmd);
}