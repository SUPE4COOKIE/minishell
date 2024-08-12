/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:02:25 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/10 21:06:36 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*var_finder(char *var, char **envp)
{
	int	i;
	int	j;

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

int	var_replace_manager(t_lexer **lex, size_t *i, char **var)
{
	if (*var)
		return (existing_var(lex, i, var));
	else
		return (inexistant_var(lex, i, var));
	if (!(*lex)->value || ((*lex)->value && !(*lex)->value[0]))
	{
		*lex = delete_lexer(lex, *lex);
		return (1);
	}
	return (2);
}

int	var_manager(t_lexer **lex, size_t *i, char **var, int last_exit_status)
{
	if ((*lex)->value[*i] == '$' && (*lex)->value[*i + 1]
		&& (ft_isalpha((*lex)->value[*i + 1]) || (*lex)->value[*i + 1] == '_'))
	{
		return (var_replace_manager(lex, i, var));
	}
	else if ((*lex)->value[*i] == '$' && (*lex)->value[*i + 1] == '?')
	{
		(*i)++;
		*var = ft_itoa(last_exit_status);
		if (!(*var))
			return (-1);
		(*lex)->value = var_replacer(*lex, *var, i);
		if (!(*lex)->value)
			return (free(var), -1);
		(*i)--;
		free(*var);
		return (2);
	}
	(*i)++;
	return (0);
}

int	lexer_iter(t_lexer **lex, size_t *i, char **envp, int last_exit_status)
{
	char	*var;
	int		var_return;

	while (*lex && ((*lex)->value && ft_strlen((*lex)->value) > (*i)))
	{
		if (get_var_value(lex, &var, i, envp) == -1)
			return (-1);
		var_return = var_manager(lex, i, &var, last_exit_status);
		if (var_return == 1)
			break ;
		else if (var_return == 2)
			continue ;
		else if (var_return == -1)
			return (-1);
	}
	return (0);
}

int	expand(t_lexer **lex, char **envp, int last_exit_status)
{
	t_lexer	*tmp;
	size_t	i;
	int		return_value;

	tmp = *lex;
	return_value = 0;
	while (tmp)
	{
		i = 0;
		if (tmp && (tmp->type == T_WORD || tmp->type == T_D_QUOTED_WORD))
		{
			return_value = lexer_iter(&tmp, &i, envp, last_exit_status);
		}
		if (tmp)
		{
			if (tmp->next == NULL || return_value == -1)
			{
				*lex = get_first_lexer(tmp);
				break ;
			}
			tmp = tmp->next;
		}
	}
	return (return_value);
}
