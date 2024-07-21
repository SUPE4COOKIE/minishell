/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:02:25 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/07/21 18:04:27 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	env variable should start with underscore or alpha
	and do not contain any special characters
*/

#include "minishell.h"

t_lexer	*lexer_replacer(t_lexer *lex, char *value, t_lexer **origin)
{
	t_lexer	*tmp;
	t_lexer	*before;
	t_lexer	*after;
	t_lexer	*last;

	tmp = lexer(value);
	if (!tmp)
		return (NULL);
	before = lex->prev;
	after = lex->next;
	if (before)
		before->next = tmp;
	tmp->prev = before;
	last = get_last_lexer(tmp);
	last->next = after;
	last->space_after = lex->space_after;
	if (after)
		after->prev = last;
	free(lex);
	if (!before)
		*origin = tmp;
	return (last);
}

void	skip_var(char **var_value)
{
	while (**var_value && **var_value == '$')
		(*var_value)++;
	if (**var_value && **var_value != '?')
	{
		while (ft_isalpha(**var_value) || **var_value == '_' || ft_isdigit(**var_value))
		{
			(*var_value)++;
			if (ft_isdigit(*((*var_value) - 1)) && *((*var_value) - 2) == '$')
				break;
		}
	}
	else if (**var_value == '?')
		(*var_value)++;
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

void	copy_var(char **result, char **var_value, size_t *i)
{
	while (**var_value)
	{
		(*result)[*i] = **var_value;
		(*i)++;
		(*var_value)++;
	}
}

char	*var_replacer(t_lexer *var, char *value, size_t *iter)
{
	char *result;
	char *var_start;
	size_t i;

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
			break;
		}
		result[i] = *(var->value);
		i++;
		(var->value)++;
	}
	copy_var(&result, &(var->value), &i);
	result[i] = 0;
	free(var_start);
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
		return (NULL);
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



int expoud(t_lexer **lex, char **envp, int last_exit_status)
{
	t_lexer	*tmp;
	size_t	i;
	char	*var;
	char	*var_name;
	char	*replaced;

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
						return (-1);
					var = var_finder(var_name, envp);
					free(var_name);
					i--;
					if (var)
					{
						if (tmp->type == T_WORD && var && contain_spaced_words(var))
						{
							replaced = var_replacer(tmp, var, &i);
							if (!replaced)
								return (free(var), -1);
							tmp = lexer_replacer(tmp, replaced, lex);
							if (!tmp)
								return (free(var), free(replaced), -1);
							free(replaced);
							break;
						}
						else
						{
							tmp->value = var_replacer(tmp, var, &i);
							if (!tmp->value)
								return (free(var), -1);
						}
					}
					else
					{
						if (tmp->type == T_WORD && var && contain_spaced_words(var))
						{
							tmp = lexer_replacer(tmp, NULL, lex);
							if (!tmp)
								return (free(var), -1);
							break;
						}
						else
						{
							tmp->value = var_replacer(tmp, var, &i);
							if (!tmp->value)
								return (free(var), -1);
						}
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
					if (!var)
						return (-1);
					tmp->value = var_replacer(tmp, var, &i);
					if (!tmp->value)
						return (free(var), -1);
					i--;
					free(var);
					continue;
				}
				i++;
			}
		}
		if (tmp)
			tmp = tmp->next;
	}
	return (0);
}

int	get_var_value(t_lexer **lex, char **var, size_t *i, char **envp)
{
	char	*var_name;
	t_lexer	*tmp;

	tmp = *lex;
	printf("tmp->value[*i] = %c\n", tmp->value[*i]);
	if (tmp->value[*i] == '$' && tmp->value[*i + 1] && 
			(ft_isalpha(tmp->value[*i + 1]) || tmp->value[*i + 1] == '_'))
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

int existing_var(t_lexer **lex, size_t *i, char **var)
{
	t_lexer	*tmp = *lex;
	char	*replaced;

	if (tmp->type == T_WORD && *var && contain_spaced_words(*var))
	{
		replaced = var_replacer(tmp, *var, i);
		if (!replaced)
			return (free(*var), -1);
		tmp = lexer_replacer(tmp, replaced, lex);
		if (!tmp)
			return (free(*var), free(replaced), -1);
		free(replaced);
		return (1);
	}
	else
	{
		tmp->value = var_replacer(tmp, *var, i);
		if (!tmp->value)
			return (free(*var), -1);
	}
	return (0);
}

int inexistant_var(t_lexer **lex, size_t *i, char **var)
{
	t_lexer	*tmp = *lex;

	if (tmp->type == T_WORD && *var && contain_spaced_words(*var))
	{
		tmp = lexer_replacer(tmp, NULL, lex);
		if (!tmp)
			return (free(*var), -1);
		return (1);
	}
	else
	{
		tmp->value = var_replacer(tmp, *var, i);
		if (!tmp->value)
			return (free(*var), -1);
	}
	return (0);
}

int	var_replace_manager(t_lexer **lex, size_t *i, char **var)
{
	t_lexer	*tmp;

	tmp = *lex;
	if (*var)
		return (existing_var(lex, i, var));
	else
		return (inexistant_var(lex, i, var));
	if (!tmp->value || (tmp->value && !tmp->value[0]))
	{
		tmp = delete_lexer(lex, tmp);
		return (1);
	}
	return (2);
}

int var_manager(t_lexer **lex, size_t *i, char **var, int last_exit_status)
{
	t_lexer *tmp;

	tmp = *lex;
	if (tmp->value[*i] == '$' && tmp->value[*i + 1] && 
			(ft_isalpha(tmp->value[*i + 1]) || tmp->value[*i + 1] == '_'))
	{
		return (var_replace_manager(lex, i, var));
	}
	else if (tmp->value[*i] == '$' && tmp->value[*i + 1] == '?')
	{
		(*i)++;
		*var = ft_itoa(last_exit_status);
		if (!(*var))
			return (-1);
		tmp->value = var_replacer(tmp, *var, i);
		if (!tmp->value)
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
	t_lexer	*tmp;
	char	*var;
	int		var_return;

	tmp = *lex;
	while (tmp && (tmp->value && ft_strlen(tmp->value) > (*i)))
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

int expand(t_lexer **lex, char **envp, int last_exit_status)
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
			return_value = lexer_iter(lex, &i, envp, last_exit_status);
		if (tmp)
			tmp = tmp->next;
	}
	return (return_value);
}
