/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 05:31:43 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/10 21:00:12 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (cmd->hdoc)
	{
		printf("hdoc: ");
		for (int i = 0; (cmd->hdoc[i]); i++)
			printf("%s , ", *(cmd->hdoc[i]));
	}
	printf("\n");
	(void)cmd;
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
bool	save_hdoc(t_minishell *mshell, t_cmd *cmd)
{
	(void)mshell;
	size_t	i;
	size_t	j;
	size_t	k;

	while (cmd)
	{
		cmd->hdoc = ft_calloc(sizeof(char **) , (ft_tablen(cmd->infile) + 1));
		i = 0;
		j = 0;
		k = 0;
		while (cmd->type_chain && cmd->type_chain[i] && cmd->type_chain[i] != UNDEFINED)
		{
			if (cmd->type_chain[i] == HDOC || cmd->type_chain[i] == RED_IN)
			{
				if (cmd->type_chain[i] == HDOC)
				{
					cmd->hdoc[j] = &(cmd->infile[k]);
					j++;
				}
				k++;
			}
			i++;
		}
		cmd = cmd->next;
	}
	return (true);
}

int	parse(t_minishell *mshell)
{
	t_lexer	*lex;

	lex = lexer(mshell->line);
	if (expand(&lex, mshell->env, mshell->last_exit_status) == -1)
		return (free_lexer(lex), -1);
	if (!validate(lex, &(mshell->last_exit_status)))
	{
		free_lexer(lex);
		return (1);
	}
	mshell->cmds = lexer_to_cmd(lex, mshell->path, &(mshell->last_exit_status));
	if (!mshell->cmds)
	{
		free_cmds(mshell->cmds);
		return (1);
	}
	save_hdoc(mshell, mshell->cmds);
	print_cmds(mshell->cmds);
	free_lexer(lex);
	return (0);
}
