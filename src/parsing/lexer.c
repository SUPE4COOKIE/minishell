/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:35:17 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/05/07 20:01:17 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	new_lexer(t_lexer **lex, char *line, size_t size)
{
	t_lexer	*new;
	
	new = malloc(sizeof(t_lexer));
	if (new == NULL)
		return (1); //TODO: free exit
	new->value = malloc(size + 1);
	if (new->value == NULL)
		return (1); //TODO: free exit
	ft_strlcpy(new->value, line, size + 1);
	if (new->value[0] == '|')
		new->type = T_PIPE;
	else if (new->value[0] == '<')
		new->type = T_REDIR_IN;
	else if (new->value[0] == '>')
		new->type = T_REDIR_OUT;
	else
		new->type = T_WORD;
	new->next = NULL;
	if (*lex == NULL)
		*lex = new;
	else
	{
		t_lexer *temp = *lex;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
	return (0);
}

void	print_lexer(t_lexer *lex)
{
	while (lex)
	{
		printf("value: %s\n", lex->value);
		printf("type: %d\n", lex->type);
		lex = lex->next;
	}
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
			{
				new_lexer(&lex, line + start, end - start);
			}
			start = end;
		}
		end++;
	}
	if (end > start)
		new_lexer(&lex, line + start, end - start);
	return (lex);
}