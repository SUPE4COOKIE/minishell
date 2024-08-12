/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_str_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 18:13:26 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/10 21:02:42 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_name(char *str)
{
	char	*name;
	size_t	i;

	i = 0;
	while (str[i] && (ft_isalpha(str[i]) || str[i] == '_'
			|| ft_isdigit(str[i])))
		i++;
	name = ft_substr(str, 0, i);
	if (!name)
		return (NULL);
	return (name);
}

void	copy_value(char **result, char **value, size_t *iter, size_t *i)
{
	while (*value && **value)
	{
		(*result)[*i] = **value;
		(*iter)++;
		(*i)++;
		(*value)++;
	}
}

void	skip_var(char **var_value)
{
	while (**var_value && **var_value == '$')
		(*var_value)++;
	if (**var_value && **var_value != '?')
	{
		while (ft_isalpha(**var_value) || **var_value == '_'
			|| ft_isdigit(**var_value))
		{
			(*var_value)++;
			if (ft_isdigit(*((*var_value) - 1)) && *((*var_value) - 2) == '$')
				break ;
		}
	}
	else if (**var_value == '?')
		(*var_value)++;
}
