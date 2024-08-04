/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_to_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 17:23:05 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/04 11:15:59 by mwojtasi         ###   ########.fr       */
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
	return ("UNDEFINED");
}

void free_str_array(char **array)
{
	size_t i;

	if (!array)
		return;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->cmd)
		free_null(cmd->cmd);
	if (cmd->args)
		free_str_array(cmd->args);
	if (cmd->op_type[0] != UNDEFINED && cmd->infile)
		free_str_array(cmd->infile);
	if (cmd->op_type[1] != UNDEFINED && cmd->outfile)
		free_str_array(cmd->outfile);
	if (cmd->type_chain)
		free_null(cmd->type_chain);
	free_null(cmd);
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

void redir_before_init(size_t *i, size_t *redir_in, size_t *redir_out, bool *reference_found)
{
	*i = 0;
	*redir_in = 0;
	*redir_out = 0;
	*reference_found = false;
}

void	is_redir_reference(char *redir, char *reference, bool *reference_found)
{
	if (redir == reference)
		*reference_found = true;
}

bool	is_redir_before(t_cmd *cmd, char **redir, char **reference)
{
	size_t	i;
	size_t	redir_in;
	size_t	redir_out;
	bool	reference_found;
	
	redir_before_init(&i, &redir_in, &redir_out, &reference_found);
	while (cmd->type_chain[i] != UNDEFINED)
	{
		if (cmd->type_chain[i] == RED_IN || cmd->type_chain[i] == HDOC)
		{
			is_redir_reference(cmd->infile[redir_in], *reference, &reference_found);
			if (cmd->infile[redir_in] == *redir && !reference_found)
				return (true);
			redir_in++;
		}
		else
		{
			is_redir_reference(cmd->outfile[redir_out], *reference, &reference_found);
			if (cmd->outfile[redir_out] == *redir && !reference_found)
				return (true);
			redir_out++;
		}
		i++;
	}
	return (false);
}

t_cmd	*new_cmd(char **args)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	if (args)
	{
		new->args = args;
		new->cmd = args[0];
	}
	else
	{
		new->args = NULL;
		new->cmd = NULL;
		new->is_valid_cmd = false;
	}
	new->infile = NULL;
	new->op_type[0] = UNDEFINED;
	new->outfile = NULL;
	new->op_type[1] = UNDEFINED;
	new->next = NULL;
	new->prev = NULL;
	new->type_chain = NULL;
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
	while (lex && lex->type != T_PIPE)
	{
		if (lex->value && (lex->type == T_WORD || lex->type == T_S_QUOTED_WORD
			|| lex->type == T_D_QUOTED_WORD))
			num++;
		if (lex->value && (lex->type == T_REDIR_IN || lex->type == T_REDIR_OUT
			|| lex->type == T_APPEND_OUT))
			num--;
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

void	print_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;

	tmp = cmd;
	while (tmp)
	{
		print_cmd(tmp);
		tmp = tmp->next;
	}
}

void	print_types(t_cmd_type *type)
{
	int	i;

	i = 0;
	while (type[i] != UNDEFINED)
	{
		printf("%s, ", cmd_type_to_str(type[i]));
		i++;
	}
	printf("\n");
}

void	print_cmd(t_cmd *cmd) // will be deleted later
{
	printf("cmd: %s\n", cmd->cmd);
	printf("args: ");
	if (cmd->args)
	{
		for (int i = 0; cmd->args[i]; i++)
			printf("%s, ", cmd->args[i]);
	}
	else
		printf("(null)");
	printf("\ninfile op type: %s\n", cmd_type_to_str(cmd->op_type[0]));
	printf("outfile op type: %s\n", cmd_type_to_str(cmd->op_type[1]));
	if (cmd->op_type[0] != UNDEFINED)
	{
		printf("infile: ");
		for (int i = 0; cmd->infile[i]; i++)
			printf("%s , ", cmd->infile[i]);
		printf("\n");
	}
	if (cmd->op_type[1] != UNDEFINED)
	{
		printf("outfile: ");
		for (int i = 0; cmd->outfile[i]; i++)
			printf("%s , ", cmd->outfile[i]);
		printf("\n");
	
	}
	if (cmd->op_type[0] != UNDEFINED || cmd->op_type[1] != UNDEFINED)
	{
		printf("type chain: ");
		print_types(cmd->type_chain);
	}
	printf("\n");
	(void)cmd;
}

int append_new_typechain(t_cmd_type **type_chain, t_cmd_type type)
{
	*type_chain = malloc(sizeof(t_cmd_type) * 2);
	if (!*type_chain)
		return (-1);
	(*type_chain)[0] = type;
	(*type_chain)[1] = UNDEFINED;
	return (0);
}

int	append_type(t_cmd_type **type_chain, t_cmd_type type)
{
	t_cmd_type	*new;
	int			i;

	i = 0;
	if (!type_chain || !*type_chain)
		return (append_new_typechain(type_chain, type));
	while ((*type_chain)[i] != UNDEFINED)
		i++;
	new = malloc(sizeof(t_cmd_type) * (i + 2));
	if (!new)
		return (-1);
	i = 0;
	while ((*type_chain)[i] != UNDEFINED)
	{
		new[i] = (*type_chain)[i];
		i++;
	}
	new[i] = type;
	new[i + 1] = UNDEFINED;
	free_null(*type_chain);
	*type_chain = new;
	return (0);
}

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

bool	is_nospace_addable(t_token_type type)
{
	if (type == T_WORD || type == T_S_QUOTED_WORD || type == T_D_QUOTED_WORD)
		return (true);
	return (false);
}

int	nospace_add(char **args, t_lexer **lex)
{
	size_t	size;
	t_lexer *start;

	size = 0;
	start = *lex;
	while (*lex && (*lex)->value && (!size || (!(*lex)->prev->space_after)) && is_nospace_addable((*lex)->type))
	{
		size += ft_strlen((*lex)->value) + 1;
		*lex = (*lex)->next;
	}
	*lex = start;
	*args = ft_calloc(size + 1, sizeof(char));
	if (!*args)
		return (-1);
	while (*lex && (*lex)->value && !(*lex)->space_after && is_nospace_addable((*lex)->type))
	{
		ft_strlcat(*args, (*lex)->value, size + 1);
		*lex = (*lex)->next;
	}
	if (*lex && (*lex)->value)
		ft_strlcat(*args, (*lex)->value, size + 1);
	return (0);
}

int	allocate_args_cmd(t_lexer *lex, char ***args, char ***args_start, t_cmd **last_cmd)
{
	int	word_num;

	word_num = get_word_num(lex);
	*args = ft_calloc(word_num + 1, sizeof(char *));
	*args_start = *args;
	if (!*args)
		return (-1);
	*last_cmd = new_cmd(NULL);
    if (!*last_cmd)
        return (free(*args), -1);
	return (0);
}

bool is_redir(t_token_type type)
{
	return (type == T_REDIR_IN || type == T_REDIR_OUT
		|| type == T_APPEND_OUT || type == T_HERE_DOC);
}

int	handle_redirections(t_cmd *last_cmd, t_lexer **lex)
{
	if (is_redir((*lex)->type))
	{
		if (append_redir(last_cmd, lex) == -1)
			return (-1);
	}
	return (0);
}

int	handle_args(char ***args, t_lexer **lex)
{
	if (!(*lex)->space_after && (*lex)->next &&
		is_nospace_addable((*lex)->next->type))
	{
		if (nospace_add(*args, lex))
			return (-1);
	}
	else
	{
		**args = ft_strdup((*lex)->value);
		if (!**args)
			return (-1);
	}
	(*args)++;
	return (0);
}

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
		get_cmd_path(&tmp, path, exit_status);
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
