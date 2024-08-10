/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_order.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 16:38:22 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/10 21:48:03 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redir_before_init(size_t *i, size_t *redir_in,
		size_t *redir_out, bool *reference_found)
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
	bool	ref_found;

	redir_before_init(&i, &redir_in, &redir_out, &ref_found);
	while (cmd->type_chain[i] != UNDEFINED)
	{
		if (cmd->type_chain[i] == RED_IN || cmd->type_chain[i] == HDOC)
		{
			is_redir_reference(cmd->infile[redir_in], *reference, &ref_found);
			if (cmd->infile[redir_in] == *redir && !ref_found)
				return (true);
			redir_in++;
		}
		else
		{
			is_redir_reference(cmd->outfile[redir_out], *reference, &ref_found);
			if (cmd->outfile[redir_out] == *redir && !ref_found)
				return (true);
			redir_out++;
		}
		i++;
	}
	return (false);
}

int	append_new_typechain(t_cmd_type **type_chain, t_cmd_type type)
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
	free(*type_chain);
	*type_chain = new;
	return (0);
}
