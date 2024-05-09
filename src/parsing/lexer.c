/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:35:17 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/05/09 22:13:26 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

void add_lexer_type(t_lexer **new, t_lexer **lex)
{
	t_lexer *tmp;

	if ((*new)->value[0] == '|')
		(*new)->type = T_PIPE;
	else if ((*new)->value[0] == '<')
		(*new)->type = T_REDIR_IN;
	else if ((*new)->value[0] == '>')
		(*new)->type = T_REDIR_OUT;
	else
		(*new)->type = T_WORD;
	(*new)->next = NULL;
	if (*lex == NULL)
		*lex = *new;
	else
	{
		tmp = *lex;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = *new;
	}
}


int is_n_only_spaces(char *line, size_t size)
{
	size_t i;

	i = 0;
	while (i < size)
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' 
			&& line[i] != '\v' && line[i] != '\f' && line[i] != '\r')
			return (0);
		i++;
	}
	return (1);
}

int	new_lexer(t_lexer **lex, char *line, size_t size)
{
	t_lexer	*new;
	char	*trim;
	
	if (line && line[0] == '\0')
		return (0);
	if (is_n_only_spaces(line, size))
		return (0);
	new = malloc(sizeof(t_lexer));
	if (new == NULL)
		return (1); //TODO: free exit
	new->value = malloc(size + 1);
	if (new->value == NULL)
		return (1); //TODO: free exit
	ft_strlcpy(new->value, line, size + 1); //TODO: check return
	trim = new->value;
	new->value = ft_strtrim(new->value, " "); // TODO: check return
	free(trim);
	add_lexer_type(&new, lex);
	return (0);
}

void	print_lexer(t_lexer *lex)
{
	while (lex)
	{
		printf("value: %s\n", lex->value);
		printf("type: %d\n\n", lex->type);
		lex = lex->next;
	}
}

void	free_lexer(t_lexer *lex)
{
	t_lexer *temp;

	while (lex)
	{
		temp = lex;
		lex = lex->next;
		free(temp->value);
		free(temp);
	}
}

void add_operator(t_lexer **lex, char *line, size_t *end)
{
	size_t len;

	len = 0;
	while (line[len] && is_special_char(line[len]) && line[0] == line[len] && len < 2)
	{
		len++;
		if (line[0] == '|')
			break ;
	}
	new_lexer(lex, line, len);
	(*end) += len;
}

t_lexer	*lexer(char *line)
{
	t_lexer *lex;
	size_t	end;
	size_t	start;

	end = 0;
	start = 0;
	lex = NULL;
	while (line[end])
	{
		if (is_special_char(line[end]))
		{
			if (end > start)
				new_lexer(&lex, line + start, end - start);
			start = end;
			while (line[end] && is_special_char(line[end]))
			{
				add_operator(&lex, line + start, &end);
				start = end;
			}
		}
		end++;
	}
	if (end > start)
		new_lexer(&lex, line + start, end - start);
	return (lex);
}