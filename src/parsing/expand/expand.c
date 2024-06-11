/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:02:25 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/06/11 20:00:49 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	env variable should start with underscore or alpha
	and do not contain any special characters
*/

#include "minishell.h"

char	*var_replacer(char *var, char *value, size_t *iter)
{
	char *result;
	size_t i;
	
	result = malloc(ft_strlen(var) + ft_strlen(value) + 1);
	i = 0;
	while (*var)
	{
		if (*var == '$' && *(var++))
		{
			while (*var && *var == '$')
				var++;
			if (*var && *var != '?')
			{
				while ((ft_isalpha(*var) || *var == '_' || ft_isdigit(*var)))
				{				
					var++;
					if (ft_isdigit(*(var - 1)) && *(var - 2) == '$')
						break;
				}
			}
			else if (*var == '?')
				var++;
			while (value && *value)
			{
				result[i] = *value;
				(*iter)++;
				i++;
				value++;
			}
			break;
		}
		else
		{
			result[i] = *var;
			i++;
			var++;
		}
	}
	while (*var)
	{
		result[i] = *var;
		i++;
		var++;
	}
	result[i] = 0;
	return (result);
}

char *var_finder(char *var, char **envp)
{
	int i;
	int j;

	i = 0;
	if (ft_strncmp(var, "UID", 3) == 0)
		return ("COUCOU");
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] == var[j] && envp[i][j] != '=')
			j++;
		if (envp[i][j] == '=' && !var[j])
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
	while (str[i] && (ft_isalpha(str[i]) || str[i] == '_' || ft_isdigit(str[i])))
		i++;
	name = ft_substr(str, 0, i);
	if (!name)
		exit(1); // TODO: add a proper exit struct
	return (name);
}

int expand(t_lexer **lex, char **envp, int last_exit_status)
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
			while (tmp->value && tmp->value[i])
			{
				if (tmp->value[i] == '$' && tmp->value[i + 1] && (ft_isalpha(tmp->value[i + 1]) || tmp->value[i + 1] == '_'))
				{
					i++;
					var_name = get_name(tmp->value + i);
					if (!var_name)
						exit(1); // TODO: add a proper exit struct
					var = var_finder(var_name, envp);
					free(var_name);
					var_name = tmp->value;
					i--;
					if (var)
						tmp->value = var_replacer(tmp->value, var, &i);
					else
						tmp->value = var_replacer(tmp->value, NULL, &i); // TODO: delete the node if null
					free(var_name);
					continue;
				}
				else if (tmp->value[i] == '$' && tmp->value[i + 1] == '?')
				{
					i++;
					var = ft_itoa(last_exit_status);
					tmp->value = var_replacer(tmp->value, var, &i);
					free(var);
					continue;
				}
				else if (tmp->value[i] == '$' && tmp->value[i + 1])
					tmp->value = var_replacer(tmp->value, NULL, &i);
				i++;
			}
		}
		tmp = tmp->next;
	}
	return (0);
}