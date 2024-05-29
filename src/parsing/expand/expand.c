/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:02:25 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/05/29 19:03:26 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

	i = 0;
	tmp = *lex;
	while (tmp)
	{
		if (tmp->type == T_WORD)
		{
			while (tmp->value[i])
			{
				if (tmp->value[i] == '$')
				{
					if (tmp->value[i + 1] == '?')
					{
						tmp->value = ft_strjoin(tmp->value, ft_itoa(g_exit_status));
						tmp->value = ft_strjoin(tmp->value, tmp->value + i + 2);
					}
					else
					{
						tmp->value = ft_strjoin(tmp->value, var_finder(tmp->value + i + 1, envp));
						tmp->value = ft_strjoin(tmp->value, tmp->value + i + 1);
					}
				}
				i++;
			}
		}
		tmp = tmp->next;
	}
	return (0);
}