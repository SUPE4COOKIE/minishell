/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:02:25 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/05/31 18:41:06 by mwojtasi         ###   ########.fr       */
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

char	*get_name(char *str)
{
	char	*name;
	size_t	i;

	i = 0;
	while (str[i] && (ft_isalpha(str[i]) || str[i] == '_'))
		i++;
	name = ft_substr(str, 0, i);
	if (!name)
		exit(1); // TODO: add a proper exit struct
	return (name);
}

int expand(t_lexer **lex, char **envp)
{
	t_lexer	*tmp;
	size_t	i;
	char	*var;
	char	*var_name;

	tmp = *lex;
	var = NULL;
	var_name = NULL;
	while (tmp)
	{
		i = 0;
		if (tmp->type == T_WORD || tmp->type == T_D_QUOTED_WORD)
		{
			while (tmp->value[i])
			{
				if (tmp->value[i] == '$' && tmp->value[i + 1] && (ft_isalpha(tmp->value[i + 1]) || tmp->value[i + 1] == '_'))
				{
					i++;
					var_name = get_name(tmp->value + i);
					if (!var_name)
						exit(1); // TODO: add a proper exit struct
					var = var_finder(var_name, envp);
					if (var)
					{
						printf("var: %s\n", var);
						// replace var_name with var
					}
					free(var_name);
				}
				i++;
			}
		}
		tmp = tmp->next;
	}
	return (0);
}