/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:02:25 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/05/30 15:44:03 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	env variable should start with underscore or alpha
	and do not contain any special characters
*/

#include "minishell.h"

char *var_finder(char *var, char **envp)
{
	int i;
	int j;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] == var[j] && envp[i][j] != '=')
			j++;
		if (envp[i][j] == '=')
			return (envp[i] + j + 1);
		i++;
	}
	return (NULL);
}

int expand(t_lexer **lex, char **envp)
{
	t_lexer *tmp;
	size_t i;
	char *var;

	i = 0;
	tmp = *lex;
	var = NULL;
	while (tmp)
	{
		if (tmp->type == T_WORD)
		{
			while (tmp->value[i])
			{
				var = var_finder(tmp->value + i, envp);
				if  (var)
				{
					printf("var: %s\n", var);
					break;
				}
				i++;
			}
		}
		tmp = tmp->next;
	}
	return (0);
}