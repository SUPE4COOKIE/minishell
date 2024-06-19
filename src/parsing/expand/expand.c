/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:02:25 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/06/19 22:46:47 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	env variable should start with underscore or alpha
	and do not contain any special characters
*/

#include "minishell.h"

char	*lexer_replacer(t_lexer *lex, char *value, t_lexer **origin)
{
	t_lexer	*tmp;
	t_lexer	*before;
	t_lexer	*after;
	t_lexer	*last;

	tmp = lexer(value);
	if (!tmp)
		exit(1); // TODO: add a proper exit struct
	before = lex->prev;
	after = lex->next;
	if (before)
		before->next = tmp;
	last = get_last_lexer(tmp);
	last->next = after;
	if (after)
		after->prev = last;
	if (!after && !before)
	{
		*origin = tmp;
	}
	return (tmp->value);
}

char	*var_replacer(t_lexer *var, char *value, size_t *iter)
{
	char *result;
	size_t i;

	result = malloc(ft_strlen(var->value) + ft_strlen(value) + 1);
	i = 0;
	while (*(var->value))
	{
		if (*(var->value) == '$' && *(var->value + 1))
		{
			while (*(var->value) && *(var->value) == '$')
				(var->value)++;
			if (*(var->value) && *(var->value) != '?')
			{
				while ((ft_isalpha(*(var->value)) || *(var->value) == '_' || ft_isdigit(*(var->value))))
				{				
					(var->value)++;
					if (ft_isdigit(*((var->value) - 1)) && *((var->value) - 2) == '$')
						break;
				}
			}
			else if (*(var->value) == '?')
				(var->value)++;
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
			result[i] = *(var->value);
			i++;
			(var->value)++;
		}
	}
	while (*(var->value))
	{
		result[i] = *(var->value);
		i++;
		(var->value)++;
	}
	result[i] = 0;
	//free(var->value);
	return (result);
}

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

bool	contain_spaced_words(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] && ft_iswhitespace(str[i]))
		i++;
	if (!str[i])
		return (false);
	while (str[i] && !ft_iswhitespace(str[i]))
		i++;
	if (!str[i])
		return (false);
	while (str[i] && ft_iswhitespace(str[i]))
		i++;
	if (!str[i])
		return (false);
	return (true);
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
		if (tmp && (tmp->type == T_WORD || tmp->type == T_D_QUOTED_WORD))
		{
			while (tmp && (tmp->value && ft_strlen(tmp->value) > i))
			{
				if (tmp->value[i] == '$' && tmp->value[i + 1] && (ft_isalpha(tmp->value[i + 1]) || tmp->value[i + 1] == '_'))
				{
					i++;
					var_name = get_name(tmp->value + i);
					if (!var_name)
						exit(1); // TODO: add a proper exit struct
					var = var_finder(var_name, envp);
					free(var_name);
					i--;
					if (var)
					{
						if (tmp->type == T_WORD && var && contain_spaced_words(var))
						{//possible memory leak
							lexer_replacer(tmp, var_replacer(tmp, var, &i), lex);
							break;
						}
						else
							tmp->value = var_replacer(tmp, var, &i);
					}
					else //humm might delete
					{
						if (tmp->type == T_WORD && var && contain_spaced_words(var))
						{//possible memory leak
							lexer_replacer(tmp, NULL, lex);
							break;
						}
						else
							tmp->value = var_replacer(tmp, NULL, &i); // TODO: delete the node if null
					}
					if (!tmp->value || (tmp->value && !tmp->value[0]))
					{
						tmp = delete_lexer(lex, tmp);
						break;
					}
					continue;
				}
				else if (tmp->value[i] == '$' && tmp->value[i + 1] == '?')
				{
					i++;
					var = ft_itoa(last_exit_status);
					tmp->value = var_replacer(tmp, var, &i);
					i--;
					free(var);
					continue;
				}
				//else if (tmp->value[i] == '$' && tmp->value[i + 1])
				//	tmp->value = var_replacer(tmp, NULL, &i);
				i++;
			}
		}
		if (tmp)
			tmp = tmp->next;
	}
	return (0);
}