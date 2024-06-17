/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:02:25 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/06/17 04:56:07 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	env variable should start with underscore or alpha
	and do not contain any special characters
*/

#include "minishell.h"

bool	is_other_cmd(t_lexer *lex)
{
	size_t	redirect_count;
	size_t	word_count;
	t_lexer	*tmp;

	tmp = lex;
	redirect_count = 0;
	word_count = 0;
	while (tmp->prev && tmp->type != T_PIPE)
	{
		if (tmp->type == T_REDIR_IN || tmp->type == T_REDIR_OUT || tmp->type == T_APPEND_OUT || tmp->type == T_HERE_DOC)
			redirect_count++;
		tmp = tmp->prev;
	}
	if (tmp->type == T_REDIR_IN || tmp->type == T_REDIR_OUT || tmp->type == T_APPEND_OUT || tmp->type == T_HERE_DOC)
		redirect_count++;
	while (tmp != lex && tmp->type != T_PIPE)
	{
		if (tmp->type == T_WORD || tmp->type == T_D_QUOTED_WORD || tmp->type == T_S_QUOTED_WORD)
			word_count++;
		tmp = tmp->next;
	}
	if (word_count > redirect_count)
		return (true);
	return (false);
}

char	*lexer_replacer(t_lexer *lex, char *value)
{
	t_lexer	*tmp;

	printf("replacing '%s' with '%s' as a cmd\n", lex->value, value);
	tmp = lexer(value);
	printf("\033[0;31mnew lexer:\n\033[0m");
	print_lexer(tmp);
	return (NULL);
}

char	*var_replacer(t_lexer *var, char *value, size_t *iter)
{
	char *result;
	size_t i;
	
	if (!is_other_cmd(var) && var->type == T_WORD)
		lexer_replacer(var, value);
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
					var_name = tmp->value;
					i--;
					if (var)
						tmp->value = var_replacer(tmp, var, &i);
					else
						tmp->value = var_replacer(tmp, NULL, &i); // TODO: delete the node if null
					if (!tmp->value || (tmp->value && !tmp->value[0]))
					{
						tmp = delete_lexer(lex, tmp);
						free(var_name);
						break;
					}
					free(var_name);
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