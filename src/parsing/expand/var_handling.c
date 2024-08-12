/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:35:01 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/10 21:08:23 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	existing_var(t_lexer **lex, size_t *i, char **var)
{
	char	*replaced;

	if ((*lex)->type == T_WORD && *var && contain_spaced_words(*var))
	{
		replaced = var_replacer(*lex, *var, i);
		if (!replaced)
			return (free(*var), -1);
		*lex = lexer_replacer(*lex, replaced, lex);
		if (!(*lex))
			return (free(*var), free(replaced), -1);
		free(replaced);
		return (1);
	}
	else
	{
		(*lex)->value = var_replacer(*lex, *var, i);
		if (!((*lex)->value))
			return (free(*var), -1);
	}
	return (0);
}

void	copy_var(char **result, char **var_value, size_t *i)
{
	while (**var_value)
	{
		(*result)[*i] = **var_value;
		(*i)++;
		(*var_value)++;
	}
}

int	inexistant_var(t_lexer **lex, size_t *i, char **var)
{
	if ((*lex)->type == T_WORD && *var && contain_spaced_words(*var))
	{
		*lex = lexer_replacer(*lex, NULL, lex);
		if (!(*lex))
			return (free(*var), -1);
		return (1);
	}
	else
	{
		(*lex)->value = var_replacer(*lex, *var, i);
		if (!(*lex)->value)
			return (free(*var), -1);
	}
	return (0);
}

int	get_var_value(t_lexer **lex, char **var, size_t *i, char **envp)
{
	char	*var_name;
	t_lexer	*tmp;

	tmp = *lex;
	if (tmp->value[*i] == '$' && tmp->value[*i + 1]
		&& (ft_isalpha(tmp->value[*i + 1]) || tmp->value[*i + 1] == '_'))
	{
		tmp = *lex;
		(*i)++;
		var_name = get_name(tmp->value + *i);
		if (!var_name)
			return (-1);
		*var = var_finder(var_name, envp);
		free(var_name);
		(*i)--;
	}
	return (0);
}

char	*var_replacer(t_lexer *var, char *value, size_t *iter)
{
	char	*result;
	char	*var_start;
	size_t	i;

	result = malloc(ft_strlen(var->value) + ft_strlen(value) + 1);
	if (!result)
		return (NULL);
	i = 0;
	var_start = var->value;
	while (*(var->value))
	{
		if (*(var->value) == '$' && *(var->value + 1))
		{
			skip_var(&(var->value));
			copy_value(&result, &value, iter, &i);
			break ;
		}
		result[i] = *(var->value);
		i++;
		(var->value)++;
	}
	copy_var(&result, &(var->value), &i);
	result[i] = 0;
	free_null(var_start);
	return (result);
}
